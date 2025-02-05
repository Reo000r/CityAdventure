#pragma once
#include "Game.h"
#include "MapChip.h"
#include <memory>

// MapChip.hに移した
/// <summary>
/// マップの元画像関連
/// </summary>
//namespace MapData
//{
//	// マップチップ一つひとつ関連
//
//
//
//	constexpr float kSizeMul = 2.0f;
//
//	// マップの画像サイズ(正方形)
//	//constexpr int kMapGraphSize = 512;
//
//	// マップチップ一つ当たりの画像サイズ
//	constexpr int kMapChipGraphSize = 32;
//	//constexpr int kMapChipGraphWidth = 32;
//	//constexpr int kMapChipGraphHeight = 32;
//
//	// マップの元画像の縦横のチップの数
//	constexpr int kMapGraphInChipNumX = 9;
//	constexpr int kMapGraphInChipNumY = 6;
//
//	// マップチップ一つ当たりの表示サイズ
//	constexpr int kChipDrawSize = kMapChipGraphSize * kSizeMul;
//	//constexpr int kChipWidth = kMapChipGraphWidth * kSizeMul;
//	//constexpr int kChipHeight = kMapChipGraphHeight * kSizeMul;
//
//	// 元データに含まれているマップチップの数
//	constexpr int kChipDataNumX = 9;
//	constexpr int kChipDataNumY = 6;
//	constexpr int kChipDataNum = kChipDataNumX * kChipDataNumY;
//
//	// マップの縦横に敷くマップチップの数
//	constexpr int kMapSizeX = 100;
//	constexpr int kMapSizeY = 20;
//	//constexpr int kMapSizeX = 25;
//	//constexpr int kMapSizeY = 15;
//}

class GameSceneCamera;

/// <summary>
/// MapChipを管理する
/// シーンだけがshared_ptrを持ち、他はweak_ptrしか持たない
/// </summary>
class Map
{
private:
	int _mapGraphHandle;

	// 当たり判定を行う際にmapが必要なので持つ
	// 他から渡してもらえばいいと思うけど修正が大変なので一旦これで
	std::weak_ptr<GameSceneCamera> _camera;

	// std::vector<std::vector<std::shared_ptr<MapChip>>>とかで
	// 持ったほうが良いと思うが、修正が大変なので一旦これで
	std::shared_ptr<MapChip> _mapChip[MapGraphData::kMapSizeY][MapGraphData::kMapSizeX];

public:
	Map();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(int mapGraphHandle, std::weak_ptr<GameSceneCamera> camera);
	//void Init(int mapGraphHandle, std::shared_ptr<GameSceneCamera> camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() const;
	//void Draw();

	/// <summary>
	/// マップの範囲内なら返し、範囲外ならnullptrを返す
	/// </summary>
	/// <param name="chipX">横番号</param>
	/// <param name="chipY">縦番号</param>
	/// <returns></returns>
	std::shared_ptr<MapChip> GetMapChipData(int chipX, int chipY) const
	{
		// マップの範囲内なら返し、範囲外ならnullptrを返す
		if (chipX >= 0 && chipX < MapGraphData::kMapSizeX &&
			chipY >= 0 && chipY < MapGraphData::kMapSizeY)
		{
			return _mapChip[chipY][chipX];
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// posとvelを加味したマップチップとの当たり判定を行う
	/// 当たっていなければvelをそのまま
	/// 当たっていれば当たらないよう調整したvelを返す
	/// 軽量化の余地あり
	/// 高速で移動してマップ突き抜けた場合は考慮していない
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vel"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	Vector2 CheckHitAllMapChip(const Vector2 pos, const Vector2 vel, const Game::Size size) const;

	/// <summary>
	/// マップチップ単体との当たり判定
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size"></param>
	/// <param name="chipX"></param>
	/// <param name="chipY"></param>
	/// <returns></returns>
	bool CheckHitMapChip(const Vector2 pos, const Game::Size size, const int chipX, const int chipY) const;
};

