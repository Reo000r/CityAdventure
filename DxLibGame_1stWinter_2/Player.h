#pragma once
#include "BoxCollider.h"
#include <memory>
#include "Game.h"

//class GameSceneCamera;
class Map;
class PlayerBulletController;

namespace PlayerConstData
{
	constexpr float kSizeMul = 2.0f;

	// キャラクターグラフィックの幅と高さ
	constexpr int kGraphWidth = 48;
	constexpr int kGraphHeight = 48;

	// 初期地点座標
	constexpr int kStartPosX = Game::kScreenCenterWidth;
	constexpr int kStartPosY = 500;//Game::kScreenCenterHeight;
	// 描画時の補正値
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 2;
	// 判定の幅と高さ
	constexpr int kColWidth = 24 * kSizeMul;
	constexpr int kColHeight = 32 * kSizeMul;
	// 当たり判定の補正値
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;//(kColHeight / 2) * -1;

	// キャラクターの幅と高さ
	constexpr int kWidth = kGraphWidth * kSizeMul;
	constexpr int kHeight = kGraphHeight * kSizeMul;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 5;

	// アニメーションのコマ数
	constexpr int kIdleAnimNum = 5;
	constexpr int kRunAnimNum  = 6;
	constexpr int kJumpAnimNum = 2;
	constexpr int kFallAnimNum = 1;

	// アニメーション一周当たりのフレーム数
	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
	constexpr int kJumpAnimTotalFrame = kJumpAnimNum * kSingleAnimFrame;
	constexpr int kFallAnimTotalFrame = kFallAnimNum * kSingleAnimFrame;

	// 移動速度
	constexpr float kMoveSpeed = 2.5f * kSizeMul;

	// ジャンプ力と重力
	constexpr float kJumpForce = -9.2f * kSizeMul;
	constexpr float kFallSpeed = 0.4f * kSizeMul;

}

class Player : public BoxCollider
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _jumpHandle;
	int _fallHandle;
	int _animFrameCount;
	bool _isHitTop;
	// 左ならtrue、右ならfalse
	bool _isReverseGraphX;
	float _fallSpeed;
	// Playerの_posは中心
	//Vector2 _pos;
	Vector2 _vel;
	// 描画時の補正値(固定値)
	// グラフィックのずれを直す為のもの
	Vector2 _drawPosOffset;

	std::shared_ptr<Map> _map;

	// 弾の発射管理を行う
	std::shared_ptr<PlayerBulletController> _bulletController;

	// UpdateのStateパターン
	// _updateが変数であることを分かりやすくしている
	using UpdateFunc_t = void(Player::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// 無操作時の更新処理
	/// </summary>
	void IdleUpdate();
	/// <summary>
	/// 左右入力時の更新処理
	/// </summary>
	void RunUpdate();
	/// <summary>
	/// 空中時の更新処理
	/// JumpUpdateとFallUpdateをまとめた
	/// </summary>
	void AirUpdate();

	// まとめたい
	/// <summary>
	/// 下側が地面に接しているか確認する
	/// </summary>
	bool CheckIsBottom() const;
	/// <summary>
	/// 上側が天井に接しているか確認する
	/// </summary>
	bool CheckIsTop();

public:
	Player();

	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::shared_ptr<Map> map, int idleHandle, int runHandle, int jumpHandle, int fallHandle);

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(GameSceneCamera camera);

	Vector2 GetPos() { return _pos; }
	//Vector2 GetCenterPos()
	//{
	//	Vector2 ans;
	//	ans.x = static_cast<int>(_pos.x);
	//	ans.y = static_cast<int>(_pos.y - kHeight * 0.5f);
	//	return ans;
	//}
};

