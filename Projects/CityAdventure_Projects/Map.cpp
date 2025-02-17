#include "Map.h"
#include "Game.h"
#include "GameSceneCamera.h"
#include "BoxCollider.h"
#include "PlatinumLoader.h"

#include <DxLib.h>
#include <cassert>

namespace
{
	constexpr unsigned int kBackGroundColor = 0x333333;
}

Map::Map() :
	_mapGraphHandle(0)
{
	PlatinumLoader& loader = PlatinumLoader::GetInstance();

	// デバッグステージを読み込むかどうか
#ifdef LOAD_DEBUG_STAGE
	loader.Load(L"data/mapdata/stage1.fmf");
#else
	loader.Load(L"data/mapdata/stage.fmf");
#endif // LOAD_DEBUG_STAGE
	
	int mapWidth = 0;
	int mapHeight = 0;
	// マップデータに置かれているチップが縦横にいくつ置かれているのかを取る
	loader.GetMapSize(mapWidth, mapHeight);

	// 違った場合は範囲外読み取りをしてしまう可能性があるので弾く
	assert(MapGraphData::kMapSizeX == mapWidth);
	assert(MapGraphData::kMapSizeY == mapHeight);

	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			// マップデータをマップチップに適用

			int mapChipNum = loader.GetChipSpriteNo(LayerType::BackGround, chipX, chipY);

			// 実際に置かれる位置
			Vector2 pos;
			pos.x = chipX * MapColData::kColSizeX;
			pos.y = chipY * MapColData::kColSizeY;

			// ポインタを作成し保存
			auto chip = std::make_shared<MapChip>(pos, mapChipNum);
			_mapChip[chipY][chipX] = chip;
		}
	}
}

void Map::Init(int mapGraphHandle, std::weak_ptr<GameSceneCamera> camera)
{
	_mapGraphHandle = mapGraphHandle;
	_camera = camera;
}

void Map::Draw() const
{
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			int chipNo = _mapChip[chipY][chipX]->GetMapChipNum();

			// 透明なら次へ
			if (chipNo == Game::kEmptyChipNum) continue;

			_mapChip[chipY][chipX]->Draw(_camera, _mapGraphHandle);
		}
	}
}

Vector2 Map::CheckHitAllMapChip(Vector2 pos, const Vector2 vel, const Game::Size size) const
{
	// 最初から0ならreturn
	if (vel.Magnitude() == 0.0f)
	{
		return vel;
	}

	Vector2 resultVel = vel;

	// 当たらなくなるまで繰り返す
	bool loop = true;
	// 初回で当たったか
	bool isFirstHit = true;
	while (loop)
	{
		loop = false;

		// 未来の位置を出す
		Vector2 futurePos = pos + resultVel;

		// 全マップチップ分繰り返す
		for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
		{
			bool isHit = false;

			for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
			{
				// 移動ベクトルの長さがなかったら
				if (resultVel.Magnitude() == 0.0f)
				{
					return resultVel;
				}

				// 当たり判定を取る前に
				// 距離的に絶対当たらない事が確定しているチップは
				// この時点でcontinue
				Vector2 chipPos = GetMapChipData(chipX, chipY)->GetPos();
				if (futurePos.Distance(futurePos, chipPos) > 800)
				{
					continue;
				}

				isHit = CheckHitMapChip(futurePos, size, chipX, chipY);

				// 初回に当たったとき
				if (isHit && isFirstHit)
				{
					// 1ドットずつ減らしたいが小数点以下の誤差が産まれるため、
					// 初回に丸め以降改めて当たり判定を行う
					// 初回なのでposもvelも丸める
					futurePos.x = floorf(futurePos.x);
					futurePos.y = floorf(futurePos.y);
					resultVel.x = floorf(resultVel.x);
					resultVel.y = floorf(resultVel.y);
					isFirstHit = false;

					// ループ継続
					loop = true;
				}

				// 当たった時点でマップチップのループからぬけるが、
				// 当たらなくなるまで繰り返すループは継続
				if (isHit && !isFirstHit)
				{
					// 当たっていた場合はvelを1ドットずつ縮めていき、当たらなくなるまで繰り返す
					// そのまま縮めると、斜めベクトルの時にジャンプの勢いも縮めてしまうため
					// 一つの成分から縮めていくことで、問題を回避する
					// 先に横(x)成分から縮めていく
					float absX = fabsf(resultVel.x);	// velのx成分の絶対値
					float absY = fabsf(resultVel.y);	// velのy成分の絶対値

					// x成分を縮め切っていなければx成分を縮める
					// x成分を縮めるかどうか
					// falseなら縮め切っているのでyを縮める
					bool shrinkX = (absX != 0.0f);

					if (shrinkX)
					{
						if (resultVel.x > 0.0f) resultVel.x -= 1.0f;
						else                    resultVel.x += 1.0f;

						// 縮め切ったら明示的に0にする
						if (fabs(resultVel.x) < 1.0f) resultVel.x = 0.0f;

						loop = true;
					}
					else
					{
						if (resultVel.y > 0.0f) resultVel.y -= 1.0f;
						else                    resultVel.y += 1.0f;

						// 縮め切ったら明示的に0にする
						if (fabs(resultVel.y) < 1.0f) resultVel.y = 0.0f;

						loop = true;
					}

					break;
				}

				/* 二重for内 */
			}
			if (isHit) break;
		}
	}

	return resultVel;
}

bool Map::CheckHitMapChip(const Vector2 pos, const Game::Size size, const int chipX, const int chipY) const
{
	std::shared_ptr<MapChip> chip = GetMapChipData(chipX, chipY);

	// 例外処理
	if (chip == nullptr)
	{
		assert(false && "範囲外読み取り");
		return false;
	}

	// 当たり判定を持たないマップチップなら判定処理を飛ばす
	int chipNo = chip->GetMapChipNum();
	if (Game::CheckEmptyMapChip(chipNo)) return false;

	Vector2 chipPos = chip->GetPos();
	Game::Size chipSize = chip->GetBoxSize();

	// 当たっているかどうか調べる
	int posTop    = pos.y - (size.height * 0.5f);
	int posBottom = pos.y + (size.height * 0.5f);
	int posRight  = pos.x + (size.width * 0.5f);
	int posLeft   = pos.x - (size.width * 0.5f);
	int chipTop    = chipPos.y - (chipSize.height * 0.5f);
	int chipBottom = chipPos.y + (chipSize.height * 0.5f);
	int chipRight  = chipPos.x + (chipSize.width * 0.5f);
	int chipLeft   = chipPos.x - (chipSize.width * 0.5f);

	// 矩形同士の当たり判定
	// 当たらないパターンを出してそれを反転させている
	bool isHit = !(
		(posTop >= chipBottom || posBottom <= chipTop) ||
		(posLeft >= chipRight || posRight <= chipLeft));
	if (isHit)
	{
		Vector2 offset = _camera.lock()->GetDrawOffset();
		
#ifdef _DEBUG

		// デバッグ描画
		DrawLine(pos.x + offset.x, pos.y + offset.y, chipPos.x + offset.x, chipPos.y + offset.y, 0xff0000);

#endif // _DEBUG

		return true;
	}
	else
	{
		return false;
	}
}
