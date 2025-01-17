#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;
class GameSceneCamera;

namespace
{
	constexpr float kSizeMul = 2.0f;

	// 使用する画像のサイズ
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;

	// 当たり判定の幅と高さ
	constexpr int kColWidth = 16 * kSizeMul;
	constexpr int kColHeight = 6 * kSizeMul;

	// 描画時の補正値
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 6;
	// 描画時の補正値(固定値)
	// グラフィックのずれを直す為のもの
	Vector2 kDrawPosOffset;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 4;

	// アニメーションのコマ数
	constexpr int kAnimNum = 4;

	// アニメーション一周当たりのフレーム数
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;

	// 移動速度
	constexpr float kMoveSpeed = 3.5f * kSizeMul;
}

/// <summary>
/// 敵の基底クラス
/// </summary>
class Enemy : public BoxCollider
{
private:
	int _graphHandle;
	int _animFrameCount;
	// 左ならtrue、右ならfalse
	bool _isReverseGraphX;

	// trueなら活性化
	// UpdateとDrawとDisActiveを受け付ける
	// falseなら非活性化
	// Activeを受け付ける
	bool _isActive;

	// _posは中心
	Vector2 _vel;

public:

	Enemy(int graphHandle);
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update(std::shared_ptr<Map> map);

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(GameSceneCamera camera);

	/// <summary>
	/// 活性化
	/// </summary>
	bool Active(Vector2 pos, bool isReverse);

	/// <summary>
	/// 非活性化
	/// </summary>
	void DisActive();

	/// <summary>
	/// 活性化状態を取得
	/// </summary>
	bool GetActiveStats() { return _isActive; }


};
