#pragma once
#include "Game.h"
#include "MapChip.h"
#include <memory>

class GameSceneCamera;

/// <summary>
/// MapChipを管理する
/// シーンだけがshared_ptrを持ち、他はweak_ptrしか持たない
/// </summary>
class Map
{
private:
	int _mapGraphHandle;

	// 当たり判定を行う際に必要なので持つ
	std::weak_ptr<GameSceneCamera> _camera;

	std::shared_ptr<MapChip> _mapChip[MapGraphData::kMapSizeY][MapGraphData::kMapSizeX];

public:
	Map();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(int mapGraphHandle, std::weak_ptr<GameSceneCamera> camera);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() const;

	/// <summary>
	/// マップの範囲内なら返し、範囲外ならnullptrを返す
	/// </summary>
	/// <param name="chipX">横番号</param>
	/// <param name="chipY">縦番号</param>
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
	Vector2 CheckHitAllMapChip(const Vector2 pos, const Vector2 vel, const Game::Size size) const;

	/// <summary>
	/// マップチップ単体との当たり判定
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size"></param>
	/// <param name="chipX"></param>
	/// <param name="chipY"></param>
	bool CheckHitMapChip(const Vector2 pos, const Game::Size size, const int chipX, const int chipY) const;
};

