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
	// 特別な事情がない限り他では使わない
	constexpr int kSizeMul = 2;

	// マップチップの判定サイズ
	constexpr int kColSizeX = MapGraphData::kGraphSizeX * kSizeMul;
	constexpr int kColSizeY = MapGraphData::kGraphSizeY * kSizeMul;
}

/// <summary>
/// マップチップ一つひとつの情報
/// まとめてMapが配列として持っている
/// </summary>
class MapChip : public BoxCollider
{
private:

	// 参照するマップチップ番号
	int _mapChipNum;

public:

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
	Vector2 GetPos() const    { return _pos; }

	/// <summary>
	/// 参照しているマップチップ番号を返す
	/// </summary>
	int GetMapChipNum() const { return _mapChipNum; }

	/// <summary>
	/// 元画像の左から数えた位置
	/// </summary>
	int GetNumOnDataX() const { return _mapChipNum % MapGraphData::kNumOfChipInDataX; }
	
	/// <summary>
	/// 元画像の上から数えた位置
	/// </summary>
	int GetNumOnDataY() const { return _mapChipNum / MapGraphData::kNumOfChipInDataX; }

	/// <summary>
	/// 使用する画像のサイズ
	/// </summary>
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
	Vector2 GetPosInGraph() const
	{
		Vector2 ans;
		ans.x = static_cast<float>(GetNumOnDataX() * MapGraphData::kGraphSizeX);
		ans.y = static_cast<float>(GetNumOnDataY() * MapGraphData::kGraphSizeY);
		return ans;
	}
};

