#pragma once
#include "UIBase.h"
#include "Vector2.h"

class Player;

namespace PlayerHitpointUIData
{
	constexpr float kUndamageSizeMul = 3.0f;
	constexpr float kDamageSizeMul   = 2.0f;

	// グラフィックの幅と高さ
	constexpr int kUndamageGraphWidth  = 16;
	constexpr int kUndamageGraphHeight = 16;
	constexpr int kDamageGraphWidth  = 16;
	constexpr int kDamageGraphHeight = 16;

	// 描画時の補正値
	constexpr int kUndamageDrawPosOffsetX = 0;
	constexpr int kUndamageDrawPosOffsetY = 0;
	constexpr int kDamageDrawPosOffsetX = 0;
	constexpr int kDamageDrawPosOffsetY = 0;
	const Vector2 kUndamageDrawPosOffset = 
	{ kUndamageDrawPosOffsetX, kUndamageDrawPosOffsetY };
	const Vector2 kDamageDrawPosOffset = 
	{ kDamageDrawPosOffsetX, kDamageDrawPosOffsetY };

	// 描画倍率を含んだ幅と高さ
	//constexpr int kWidth = kGraphWidth * kSizeMul;
	//constexpr int kHeight = kGraphHeight * kSizeMul;

	//アニメーション1コマのフレーム数
	constexpr int kUndamageSingleAnimFrame = 10;
	constexpr int kDamageSingleAnimFrame = 30;
	// 
	constexpr int kDamageEffectAnimFrame = 60;

	// アニメーションのコマ数
	constexpr int kUndamageAnimNum = 4;
	constexpr int kDamageAnimNum = 4;

	// アニメーション一周当たりのフレーム数
	constexpr int kUndamageAnimTotalFrame = kUndamageAnimNum * kUndamageSingleAnimFrame;
	constexpr int kDamageAnimTotalFrame = kDamageAnimNum * kDamageSingleAnimFrame;
}

/// <summary>
/// HP一つひとつ
/// </summary>
class PlayerHitpointUI : public UIBase
{
private:

	int _undamagedGraphHandle;
	int _damagedGraphHandle;

	// 自分が何番目のHPUIなのか保存
	// 1から始める
	int _hitpointIndex;

	// HPの最大値と現在値を保存
	int _playerMaxHitpoint;
	int _playerCurrentHitpoint;
	
	int _damageEffectCount;

	Vector2 _drawPosOffset;

	std::weak_ptr<Player> _player;

	using UpdateFunc_t = void(PlayerHitpointUI::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// ダメージを食らう前
	/// </summary>
	void UndamageUpdate();
	
	/// <summary>
	/// ダメージを食らった瞬間
	/// </summary>
	void OndamageUpdate();
	
	/// <summary>
	/// ダメージを食らった後
	/// </summary>
	void DamageUpdate();

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerHitpointUI(Vector2 pos, int hitpointIndex);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::weak_ptr<Player> player, int undamagedGraphHandle, int damagedGraphHandle);

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 活性化
	/// </summary>
	bool Active(Vector2 pos, bool isReverse) override;

	/// <summary>
	/// 非活性化
	/// </summary>
	void DisActive() override;
};

