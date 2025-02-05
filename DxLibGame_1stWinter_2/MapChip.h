#pragma once
#include "BoxCollider.h"

#include <memory>

namespace MapGraphData
{
	// 使用する画像のサイズ
	constexpr int kGraphSizeX = 32;
	constexpr int kGraphSizeY = 32;


	// マップの縦横に敷くマップチップの数
	constexpr int kMapSizeX = 100;
	constexpr int kMapSizeY = 40;

	// 元画像に含まれているチップの数
	constexpr int kNumOfChipInDataX = 9;
	constexpr int kNumOfChipInDataY = 6;
	constexpr int kNumOfAllChipInData = kNumOfChipInDataX * kNumOfChipInDataY;
}

namespace MapColData
{
	// 判定と表示の倍率
	// MapChipColData内の変数全てに掛けられているため、新たに掛ける必要はない
	// 特別な事情がない限り使わない
	constexpr int kSizeMul = 2;

	// マップチップの判定サイズ
	constexpr int kColSizeX = MapGraphData::kGraphSizeX * kSizeMul;
	constexpr int kColSizeY = MapGraphData::kGraphSizeY * kSizeMul;
}

/// <summary>
/// マップチップ一つひとつの情報
/// まとめてMapが配列として持っている
/// vector<vector<MapChip>>とかで持ったほうが良いと思うが
/// 修正が大変なので一旦これで
/// </summary>
class MapChip : public BoxCollider
{
private:

	// _pos はBoxColliderに含まれている

	// 参照するマップチップ番号
	int _mapChipNum;

public:

	// コンストラクタ(旧)
	/// <summary>
	/// コンストラクタ(旧)
	/// </summary>
	/// <param name="pos"       >ゲーム上の位置</param>
	/// <param name="posInGraph">元画像から使用する画像の位置(左上)</param>
	/// <param name="graphSize" >元画像から使用する画像のサイズ</param>
	/// <param name="colSize"   >実際の当たり判定のサイズ</param>
	/// <param name="mapChipNum">参照するマップチップ番号</param>
	/// <param name="sizeMul"   >表示倍率</param>
	//MapChip(Vector2 pos, Vector2 posInGraph, Game::Size graphSize, Game::Size colSize, int mapChipNum, float sizeMul);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos"       >ゲーム上の位置</param>
	/// <param name="mapChipNum">参照するマップチップ番号</param>
	MapChip(Vector2 pos, int mapChipNum);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera, int mapGraphHandle) const;

	/// <summary>
	/// 現在位置を返す
	/// </summary>
	/// <returns></returns>
	Vector2 GetPos() const    { return _pos; }

	/// <summary>
	/// 参照しているマップチップ番号を返す
	/// </summary>
	/// <returns></returns>
	int GetMapChipNum() const { return _mapChipNum; }

	/// <summary>
	/// 元画像の左から数えた位置
	/// </summary>
	/// <returns></returns>
	int GetNumOnDataX() const { return _mapChipNum % MapGraphData::kNumOfChipInDataX; }
	
	/// <summary>
	/// 元画像の上から数えた位置
	/// </summary>
	/// <returns></returns>
	int GetNumOnDataY() const { return _mapChipNum / MapGraphData::kNumOfChipInDataX; }

	/// <summary>
	/// 使用する画像のサイズ
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipGraphSize() const
	{
		Game::Size ans;
		ans.width  = MapGraphData::kGraphSizeX;
		ans.height = MapGraphData::kGraphSizeY;
		return ans;
	}

	/// <summary>
	/// マップチップの判定サイズ
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipColSize() const
	{
		Game::Size ans;
		ans.width  = MapColData::kColSizeX;
		ans.height = MapColData::kColSizeY;
		return ans;
	}

	/// <summary>
	/// 元画像内での位置を返す
	/// </summary>
	/// <returns></returns>
	Vector2 GetPosInGraph() const
	{
		Vector2 ans;
		ans.x = static_cast<float>(GetNumOnDataX() * MapGraphData::kGraphSizeX);
		ans.y = static_cast<float>(GetNumOnDataY() * MapGraphData::kGraphSizeY);
		return ans;
	}
};

