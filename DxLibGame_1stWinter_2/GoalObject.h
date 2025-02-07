#pragma once
#include "BoxCollider.h"

namespace GoalData
{
	
#ifdef LOAD_DEBUG_STAGE
	// 初期地点座標
	constexpr int kStartPosX = 72 * 32 * 2.0f;
	constexpr int kStartPosY = 3 * 32 * 2.0f + 4 * 2.0f;
#else
	constexpr int kStartPosX = 65 * 32 * 2.0f;
	constexpr int kStartPosY = 33 * 32 * 2.0f + 4 * 2.0f;
#endif // LOAD_DEBUG_STAGE

	// 使用する画像の幅と高さ
	constexpr int kGraphWidth  = 48;
	constexpr int kGraphHeight = 48;

	// 描画時の補正値
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = -24;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 9;

	// アニメーションのコマ数
	constexpr int kAnimNum = 4;

	// アニメーション一周当たりのフレーム数
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;;
}

namespace GoalColData
{
	// 判定と表示の倍率
	// MapColData内の変数全てに掛けられているため、新たに掛ける必要はない
	// 特別な事情がない限り使わない
	constexpr int kSizeMul = 2;

	// 判定サイズ
	constexpr int kColWidth  = 40  * kSizeMul;
	constexpr int kColHeight = 24 * kSizeMul;
}

/// <summary>
/// ゴール判定を持つオブジェクト
/// Mapと合わせたかった
/// </summary>
class GoalObject : public BoxCollider
{
	// _pos はBoxColliderに含まれている

	int _graphHandle;
	int _animFrameCount;
	bool _isReverseGraphX;

	bool _isActive;
	float _alpha;

	Vector2 _drawPosOffset;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GoalObject();

	~GoalObject();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(int graphHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera) const;

	/// <summary>
	/// 使用する画像のサイズ
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipGraphSize() const
	{
		Game::Size ans;
		ans.width = GoalData::kGraphWidth;
		ans.height = GoalData::kGraphHeight;
		return ans;
	}

	/// <summary>
	/// マップチップの判定サイズ
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipColSize() const
	{
		Game::Size ans;
		ans.width = GoalColData::kColWidth;
		ans.height = GoalColData::kColHeight;
		return ans;
	}

	bool Active();

	bool IsActive() { return _isActive; }
};

