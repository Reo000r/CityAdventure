#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;

namespace PlayerBulletData
{
	// 攻撃力
	constexpr int kAddDamageAmount = 1;



	constexpr float kSizeMul = 2.0f;

	// 使用する画像のサイズ
	constexpr int kGraphWidth  = 16;
	constexpr int kGraphHeight = 16;
	
	// 当たり判定の幅と高さ
	constexpr int kColWidth  = static_cast<int>(16 * kSizeMul);
	constexpr int kColHeight = static_cast<int>(6 * kSizeMul);

	// 描画時の補正値
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 6;

	// 生成位置の補正値
	constexpr int kActivePosOffsetX = 24 / 2 + 12;
	constexpr int kActivePosOffsetY = -8;
	// 生成位置の補正値
	const Vector2 kActivePosOffset = { kActivePosOffsetX,kActivePosOffsetY };

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 2;

	// アニメーションのコマ数
	constexpr int kAnimNum = 4;

	// アニメーション一周当たりのフレーム数
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;

	// 移動速度
	constexpr float kMoveSpeed = 4.5f * kSizeMul;

	// 持続時間
	constexpr int kLifeTime = 90;
}

class PlayerBullet : public BoxCollider
{
private:
	int _graphHandle;
	int _animFrameCount;
	// 左ならtrue、右ならfalse
	bool _isReverseGraphX;
	// 0になったら非活性化
	int _lifeTimeCount;
	
	// trueなら活性化
	// UpdateとDrawとDisActiveを受け付ける
	// falseなら非活性化
	// Activeを受け付ける
	bool _isActive;

	Vector2 _vel;
	// 描画時の補正値(固定値)
	// グラフィックのずれを直す為のもの
	Vector2 _drawPosOffset;

public:

	PlayerBullet(int graphHandle);
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update(std::weak_ptr<Map> map);

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

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
	bool GetActiveStats() const { return _isActive; }

	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	int GetDamage() const { return PlayerBulletData::kAddDamageAmount; }
};

