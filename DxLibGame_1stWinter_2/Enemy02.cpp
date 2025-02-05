#include "Enemy02.h"
#include "Map.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerBulletController.h"

#include "Input.h"

#include <cassert>

void Enemy02::FlyUpdate()
{
	// プレイヤーの一定範囲に入るまでUpdateを止める
	//Vector2 playerPos = _player.lock()->GetPos();
	//if (_pos.Distance(_pos, playerPos) >= Enemy02Data::kActivationRange) return;
	if (!CheckPlayerDistance()) return;

	_animFrameCount++;
	_flyFrameCount++;

	// アニメーションの合計フレーム数を超えたら最後で止める
	if (_animFrameCount >= Enemy02Data::kFlyAnimTotalFrame)
	{
		_animFrameCount = 0;//Enemy02Data::kFlyAnimTotalFrame;
	}

	// 弾と当たったかどうか
	Game::Rect rect = GetRect();
	// 取得用
	std::weak_ptr<PlayerBullet> bullet;
	if (_playerBulletController.lock()->IsHitBullet(rect, bullet))
	{
		// 弾が自分より左側にいればtrue
		bool isReverseX = bullet.lock()->GetPos().x < _pos.x;
		int damage = bullet.lock()->GetDamage();
		// 当たったら自分にダメージ
		OnDamage(damage, isReverseX);
	}


	// 移動
	_vel = {};
	float s = _flyFrameCount / 15.0f;
	_vel.y = cos(s);
	_vel.y *= 4.0f;


	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;

	// プレイヤーと当たり判定を行う
	IsPlayerHit();

	// 左右に壁があるかを判定
	if (IsRightAndLeft())
	{
		// あった場合は反転
		_isReverseGraphX = !_isReverseGraphX;
	}
}

void Enemy02::DeathUpdate()
{
	_deadFrameCount++;

	float maxDeadFrame = Enemy02Data::kDeathAnimTotalFrame * 2;
	
	if (_deadFrameCount > maxDeadFrame / 2.0f)
	{
		_alpha = (maxDeadFrame - _deadFrameCount / maxDeadFrame);
		_sizeMul -= 1 * Enemy02Data::kSizeMul / maxDeadFrame;
	}

	if (_deadFrameCount > maxDeadFrame)
	{
		DisActive();
		return;
	}


	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 落下速度の更新
	_fallSpeed += Enemy02Data::kFallSpeed;

	if (_fallSpeed > Enemy02Data::kMaxFallSpeed)
	{
		_fallSpeed = Enemy02Data::kMaxFallSpeed;
	}

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;
}

bool Enemy02::IsHitBottom() const
{
	// 1ドット下にずらし、当たっていれば地面についている
	Vector2 checkPos = _pos;
	checkPos.y += 1.0f;

	// 全マップチップ分当たり判定を行う
	bool isHit = false;
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// 例外処理
			if (chip == nullptr)
			{
				assert(false && "範囲外読み取り");
				continue;
			}

			// 当たり判定を持たないマップチップなら判定処理を飛ばす
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// 当たり判定を行う
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}

	return isHit;
}

bool Enemy02::IsRightAndLeft()
{
	// 右

	// 1ドット右にずらし、当たっていれば壁に触れている
	Vector2 checkPos = _pos;
	checkPos.x += 1.0f;

	// 全マップチップ分当たり判定を行う
	bool isHit = false;
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// 例外処理
			if (chip == nullptr)
			{
				assert(false && "範囲外読み取り");
				continue;
			}

			// 当たり判定を持たないマップチップなら判定処理を飛ばす
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// 当たり判定を行う
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// 右へ
				//_isReverseGraphX = false;
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}

	if (isHit)
	{
		return isHit;
	}

	// 左

	// 1ドット左にずらし、当たっていれば壁に触れている
	checkPos = _pos;
	checkPos.x -= 1.0f;

	// 全マップチップ分当たり判定を行う
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// 例外処理
			if (chip == nullptr)
			{
				assert(false && "範囲外読み取り");
				continue;
			}

			// 当たり判定を持たないマップチップなら判定処理を飛ばす
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// 当たり判定を行う
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// 左へ
				//_isReverseGraphX = true;
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}

	return isHit;
}

bool Enemy02::IsPlayerHit()
{
	// 非活性化状態なら飛ばす
	if (!_player.lock()->GetActiveStats()) return false;

	// 自分(敵)の矩形情報を取得
	Game::Rect rect = GetRect();
	// プレイヤーの当たり判定を取得
	Game::Rect playerRect = _player.lock()->GetRect();

	// 弾と当たり判定
	bool isHit = !(
		(rect.top >= playerRect.bottom || rect.bottom <= playerRect.top) ||
		(rect.left >= playerRect.right || rect.right <= playerRect.left));
	// 当たっていたら
	if (isHit)
	{
		Vector2 playerPos = _player.lock()->GetPos();
		// 自分がプレイヤーより左側にいればtrue
		bool isReverseX = _pos.x < playerPos.x;

		// プレイヤーに当たったことを伝える
		_player.lock()->OnDamage(Enemy02Data::kAddDamageAmount, isReverseX);
		return true;
	}

	return false;
}

Enemy02::Enemy02(Vector2 pos) :
	Enemy(pos, { Enemy02Data::kColPosOffsetX, Enemy02Data::kColPosOffsetY }, { Enemy02Data::kColWidth,Enemy02Data::kColHeight }, Enemy02Data::kHitPoint),
	_useHandle(0),
	_idleHandle(0),
	_runHandle(0),
	_fallHandle(0),
	_animFrameCount(0),
	_isHitTop(false),
	_flyFrameCount(0),
	_isDead(false),
	_deadFrameCount(0),
	_sizeMul(Enemy02Data::kSizeMul),
	_alpha(0.0f),
	_fallSpeed(0.0f),
	_drawPosOffset({ Enemy02Data::kDrawPosOffsetX, Enemy02Data::kDrawPosOffsetY }),
	_nowUpdateState(&Enemy02::FlyUpdate)
{
}

Enemy02::~Enemy02()
{
	DeleteGraph(_idleHandle);
	DeleteGraph(_runHandle);
	DeleteGraph(_fallHandle);
}

void Enemy02::Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int fallHandle)
{
	_map = map;
	_player = player;
	_playerBulletController = playerBulletController;

	_idleHandle = idleHandle;
	assert(_idleHandle >= 0);
	_runHandle = runHandle;
	assert(_runHandle >= 0);
	_fallHandle = fallHandle;
	assert(_fallHandle >= 0);

	_useHandle = _runHandle;
}

void Enemy02::Update()
{
	(this->*_nowUpdateState)();
}

void Enemy02::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / Enemy02Data::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * Enemy02Data::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width = Enemy02Data::kGraphWidth;
	int height = Enemy02Data::kGraphHeight;

	if (_alpha != 0.0f)
	{
		float a = _deadFrameCount / (Enemy02Data::kDeathAnimTotalFrame * 2.0f);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
		
	}
	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		_sizeMul, 0.0,
		_useHandle, true, _isReverseGraphX, false);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG

	DispCol(camera);
	Vector2 offset = camera.lock()->GetDrawOffset();
	int lineLength = 32;
	if (!_isReverseGraphX)
	{
		DrawLine(_pos.x + offset.x, _pos.y + offset.y, _pos.x + offset.x + lineLength, _pos.y + offset.y, 0xff0000);
	}
	else
	{
		DrawLine(static_cast<int>(_pos.x + offset.x),
			static_cast<int>(_pos.y + offset.y),
			static_cast<int>(_pos.x + offset.x - lineLength),
			static_cast<int>(_pos.y + offset.y),
			0xff0000);
	}


#endif // _DEBUG
}

bool Enemy02::Active(Vector2 pos, bool isReverse)
{
	if (_isActive) return false;

	_isActive = true;
	_pos = pos;
	_isReverseGraphX = isReverse;

	return true;
}

void Enemy02::DisActive()
{
	if (!_isActive) return;

	_isActive = false;
}

void Enemy02::OnDamage(int damage, bool isReverseX)
{
	_hitPoint -= damage;

	// 耐久力が0以下になったら非活性化
	if (_hitPoint <= 0)
	{
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Enemy02::DeathUpdate;
		_isDead = true;

		float s = _flyFrameCount / 15.0f;
		float fallSpeed = sin(s);
		fallSpeed *= 4.0f;
		_fallSpeed = fallSpeed;

		return;
	}
}
