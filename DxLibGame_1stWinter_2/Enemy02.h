#pragma once
#include "Enemy.h"
#include "Player.h"

namespace Enemy02Data
{
	// 耐久力
	constexpr int kHitPoint = 1;

	// 攻撃力
	constexpr int kAddDamageAmount = 1;



	constexpr float kSizeMul = 3.0f;

	// キャラクターグラフィックの幅と高さ
	constexpr int kGraphWidth  = 32;
	constexpr int kGraphHeight = 32;

	// 描画時の補正値
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = -18;
	// 判定の幅と高さ
	constexpr int kColWidth = 16 * kSizeMul;
	constexpr int kColHeight = 16 * kSizeMul;
	// 当たり判定の補正値
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;//(kColHeight / 2) * -1;

	// キャラクターの幅と高さ
	constexpr int kWidth  = kGraphWidth  * kSizeMul;
	constexpr int kHeight = kGraphHeight * kSizeMul;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 5;

	// アニメーションのコマ数
	constexpr int kFlyAnimNum = 6;
	constexpr int kDeathAnimNum = 4;

	// アニメーション一周当たりのフレーム数
	constexpr int kFlyAnimTotalFrame = kFlyAnimNum * kSingleAnimFrame;
	constexpr int kDeathAnimTotalFrame = kDeathAnimNum * kSingleAnimFrame;

	// 移動速度
	constexpr float kMoveSpeed = 2.0f * kSizeMul;

	// 重力
	constexpr float kFallSpeed = 0.4f * kSizeMul;

	// プレイヤーがこの距離内にいたら動く
	constexpr int kActivationRange = 1000;



	// 落下速度上限
	constexpr float kMaxFallSpeed = 5.0f;
}

/// <summary>
/// 空中敵
/// </summary>
class Enemy02 : public Enemy
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _fallHandle;
	int _animFrameCount;
	bool _isHitTop;

	// 滞空時間
	int _flyFrameCount;

	bool _isDead;
	int  _deadFrameCount;
	float _sizeMul;
	float _angle;
	float _alpha;

	float _fallSpeed;
	//Vector2 _pos;
	//Vector2 _vel;
	// 描画時の補正値(固定値)
	// グラフィックのずれを直す為のもの
	Vector2 _drawPosOffset;

	std::weak_ptr<Player> _player;
	std::weak_ptr<PlayerBulletController> _playerBulletController;

	// UpdateのStateパターン
	// _updateが変数であることを分かりやすくしている
	using UpdateFunc_t = void(Enemy02::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// 空中時の更新処理
	/// </summary>
	void FlyUpdate();
	/// <summary>
	/// 死亡時の更新処理
	/// </summary>
	void DeathUpdate();

	/// <summary>
	/// 下側が地面に接しているか確認する
	/// </summary>
	bool IsHitBottom() const;
	/// <summary>
	/// 右/左側が壁に接しているか確認する
	/// </summary>
	bool IsRightAndLeft();

	/// <summary>
	/// プレイヤーと当たり判定を行う
	/// </summary>
	/// <returns></returns>
	bool IsPlayerHit();

	/// <summary>
	/// プレイヤーとの距離を測る
	/// </summary>
	/// <returns>動けるならtrue</returns>
	bool CheckPlayerDistance()
	{
		Vector2 playerPos = _player.lock()->GetPos();
		float x = _pos.x * 2 - playerPos.x * 2;
		float y = _pos.y * 2 - playerPos.y * 2;
		if (x < 0.0f) x *= -1;
		if (y < 0.0f) y *= -1;
		float blockNumX = 25.0f;
		float blockNumY = 15.0f;
		int blockSize = 32;
		float blockMul = 2.0f;
		//return (_pos.Distance(_pos, playerPos) > Enemy01Data::kActivationRange);
		return (x <= blockNumX * blockSize * blockMul &&
			(y <= blockNumY * blockSize * blockMul));
	}

public:
	Enemy02(Vector2 pos);

	~Enemy02();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::weak_ptr<Map> map,
		std::weak_ptr<Player> player,
		std::weak_ptr<PlayerBulletController> playerBulletController,
		int idleHandle,
		int runHandle,
		int fallHandle) override;

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera) override;

	/// <summary>
	/// 描画全般
	/// </summary>
	bool Active(Vector2 pos, bool isReverse) override;

	/// <summary>
	/// 描画全般
	/// </summary>
	void DisActive() override;

	/// <summary>
	/// ダメージを食らった時の処理
	/// </summary>
	void OnDamage(int damage, bool isReverseX) override;


	Vector2 GetPos() const { return _pos; }
};