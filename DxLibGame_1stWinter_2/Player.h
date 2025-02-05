#pragma once
#include "BoxCollider.h"
#include <memory>
#include "Game.h"

//class GameSceneCamera;
class GamePlayScene;
class GoalObject;
class Map;
class PlayerBulletController;

namespace PlayerData
{
	// 耐久力
	constexpr int kHitPoint = 15;
	
	// 無敵時間
	constexpr int kUnHitTime = 60 * 2;

	// 落下したとみなされる高さ
	// 下へ移動
	//constexpr int kDeadHeight = 1024 + 192 + kColWidth * 0.5f;

	// 初期地点座標
	constexpr int kStartPosX = 360;
	constexpr int kStartPosY = 2240;



	constexpr float kSizeMul = 2.0f;

	// キャラクターグラフィックの幅と高さ
	constexpr int kGraphWidth = 48;
	constexpr int kGraphHeight = 48;

	// 描画時の補正値
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 2;
	// 判定の幅と高さ
	constexpr int kColWidth  = static_cast<int>(24 * kSizeMul);
	constexpr int kColHeight = static_cast<int>(32 * kSizeMul);
	// 当たり判定の補正値
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;//(kColHeight / 2) * -1;

	// キャラクターの幅と高さ
	constexpr int kWidth  = static_cast<int>(kGraphWidth * kSizeMul);
	constexpr int kHeight = static_cast<int>(kGraphHeight * kSizeMul);

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 5;
	constexpr int kDeathAnimFrame = 10;

	// アニメーションのコマ数
	constexpr int kIdleAnimNum = 5;
	constexpr int kRunAnimNum  = 6;
	constexpr int kJumpAnimNum = 2;
	constexpr int kFallAnimNum = 1;
	constexpr int kDamageAnimNum = 1;
	constexpr int kDeathAnimNum = 8;

	// アニメーション一周当たりのフレーム数
	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
	constexpr int kJumpAnimTotalFrame = kJumpAnimNum * kSingleAnimFrame;
	constexpr int kFallAnimTotalFrame = kFallAnimNum * kSingleAnimFrame;
	constexpr int kDamageAnimTotalFrame = kDamageAnimNum * kSingleAnimFrame;
	constexpr int kDeathAnimTotalFrame = kDeathAnimNum * kDeathAnimFrame;

	// 移動速度
	constexpr float kMoveSpeed = 2.5f * kSizeMul;

	// ジャンプ力と重力
	constexpr float kJumpForce = -9.2f * kSizeMul;
	constexpr float kFallSpeed = 0.4f * kSizeMul;



	// 落下したとみなされる高さ
	constexpr int kDeadHeight = static_cast<int>(2241 + 32*10 + kColWidth * 0.5f);
}

class Player : public BoxCollider
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _jumpHandle;
	int _fallHandle;
	int _deathHandle;
	int _animFrameCount;
	
	// 耐久力
	int _hitPoint;
	// 無敵時間
	int _unHitTime;
	bool _isActive;
	bool _isDead;
	bool _isGoal;
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

	// ゲームオーバーを伝えるため
	std::weak_ptr<GamePlayScene> _gamePlayScene;

	std::weak_ptr<GoalObject> _goal;

	std::weak_ptr<Map> _map;

	// 弾の発射管理を行う
	// シーンがsharedを持つため、weakで持つ
	std::weak_ptr<PlayerBulletController> _bulletController;

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
	/// <summary>
	/// 被弾時の更新処理
	/// 入力は効かず、向いている方向とは逆にノックバック
	/// </summary>
	void DamageUpdate();
	/// <summary>
	/// 死亡時の更新処理
	/// 入力は効かない
	/// </summary>
	void DeathUpdate();
	/// <summary>
	/// 落下死時の更新処理
	/// 入力は効かない
	/// </summary>
	void FallDeathUpdate();

	// まとめたい
	/// <summary>
	/// 下側が地面に接しているか確認する
	/// </summary>
	bool IsHitBottom() const;
	/// <summary>
	/// 上側が天井に接しているか確認する
	/// </summary>
	bool IsHitTop();

	/// <summary>
	/// ゴールと当たっているか確認
	/// </summary>
	bool IsHitGoal() const;
	/// <summary>
	/// 落下で場外に出たか判定する
	/// 出ていたら死
	/// </summary>
	/// <returns>trueが返ってきていたら死んでいるので即Updateを呼びreturn</returns>
	bool IsFallOutOfPlayArea();

public:
	Player();

	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(/*std::weak_ptr<GamePlayScene> gamePlayScene, */std::weak_ptr<GoalObject> goal, std::weak_ptr<Map> map, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int jumpHandle, int fallHandle, int deathHandle);

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

	/// <summary>
	/// ダメージを食らった時の処理
	/// </summary>
	/// <param name="">反転フラグと同じく、左ならtrue、右ならfalse</param>
	void OnDamage(int damage, bool isReverseX);

	Vector2 GetPos() const { return _pos; }

	bool GetActiveStats() const { return _isActive; }
	int GetMaxHitpoint() const { return PlayerData::kHitPoint; }
	int GetCurrentHitpoint() const { return _hitPoint; }

	bool IsDead() const { return _isDead; }
	bool IsGoal() const { return _isGoal; }
};

