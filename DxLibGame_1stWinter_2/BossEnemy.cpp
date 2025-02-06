#include "BossEnemy.h"
#include "Map.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerBulletController.h"
#include "EnemyBulletController.h"
#include "ScoreController.h"

#include "Input.h"

#include <cassert>

void BossEnemy::FlyUpdate()
{
	// プレイヤーの一定範囲に入るまでUpdateを止める
	//Vector2 playerPos = _player.lock()->GetPos();
	//if (_pos.Distance(_pos, playerPos) >= BossEnemyData::kActivationRange) return;
	//if (!CheckPlayerDistance()) return;

	_animFrameCount++;
	_flyFrameCount++;
	_bulletCooltimeFrame--;

	// アニメーションの合計フレーム数を超えたら最後で止める
	if (_animFrameCount >= BossEnemyData::kFlyAnimTotalFrame)
	{
		_animFrameCount = 0;//BossEnemyData::kFlyAnimTotalFrame;
	}

	// 弾と当たったかどうか
	Game::Rect rect = GetRect();
	// 取得用
	std::weak_ptr<PlayerBullet> bullet;
	if (_playerBulletController.lock()->IsHitBullet(rect, bullet))
	{
		// 弾が自分より左側にいればtrue
		bool isReverseX = bullet.lock()->GetPos().x < _pos.x;
		int damage = bullet.lock()->GetDamage();
		// 当たったら自分にダメージ
		OnDamage(damage, isReverseX);
	}


	// 移動
	_vel = {};
	float s = _flyFrameCount / 20.0f;
	_vel.x = sin(s);
	_vel.y = cos(s);
	_vel *= BossEnemyData::kRadMul;

	if (_isReverseGraphX)_vel *= -1;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	//_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;

	// 弾を出す
	if (_bulletCooltimeFrame <= 0 &&
		CheckPlayerDistance())
	{
		_bulletCooltimeFrame = BossEnemyData::kBulletCooltime;
		_enemyBulletController.lock()->AddBullet(_pos, false);
	}

	// プレイヤーと当たり判定を行う
	IsPlayerHit();
}

void BossEnemy::DeathUpdate()
{
	_deadFrameCount++;

	float maxDeadFrame = BossEnemyData::kDeathAnimTotalFrame * 2;
	_sizeMul -= 1 * BossEnemyData::kSizeMul / maxDeadFrame;
	_angle += (_deadFrameCount / maxDeadFrame);
	//if (_deadFrameCount > maxDeadFrame / 2.0f)
	//{
	//	_alpha = (maxDeadFrame - _deadFrameCount / maxDeadFrame);
	//	_sizeMul -= 1 * BossEnemyData::kSizeMul / maxDeadFrame;
	//}

	if (_deadFrameCount > maxDeadFrame)
	{
		DisActive();
		return;
	}


	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 落下速度の更新
	_fallSpeed += BossEnemyData::kFallSpeed;

	if (_fallSpeed > BossEnemyData::kMaxFallSpeed)
	{
		_fallSpeed = _fallSpeed = BossEnemyData::kMaxFallSpeed;
	}

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;
}

bool BossEnemy::IsPlayerHit()
{
	// 非活性化状態なら飛ばす
	if (!_player.lock()->GetActiveStats()) return false;

	// 自分(敵)の矩形情報を取得
	Game::Rect rect = GetRect();
	// プレイヤーの当たり判定を取得
	Game::Rect playerRect = _player.lock()->GetRect();

	// 弾と当たり判定
	bool isHit = !(
		(rect.top >= playerRect.bottom || rect.bottom <= playerRect.top) ||
		(rect.left >= playerRect.right || rect.right <= playerRect.left));
	// 当たっていたら
	if (isHit)
	{
		Vector2 playerPos = _player.lock()->GetPos();
		// 自分がプレイヤーより左側にいればtrue
		bool isReverseX = _pos.x < playerPos.x;

		// プレイヤーに当たったことを伝える
		_player.lock()->OnDamage(BossEnemyData::kAddDamageAmount, isReverseX);
		return true;
	}

	return false;
}

BossEnemy::BossEnemy(Vector2 pos) :
	Enemy(pos, { BossEnemyData::kColPosOffsetX, BossEnemyData::kColPosOffsetY }, { BossEnemyData::kColWidth,BossEnemyData::kColHeight }, BossEnemyData::kHitPoint),
	_useHandle(0),
	_idleHandle(0),
	_runHandle(0),
	_fallHandle(0),
	_animFrameCount(0),
	_isHitTop(false),
	_flyFrameCount(0),
	_bulletCooltimeFrame(0),
	_isDead(false),
	_deadFrameCount(0),
	_sizeMul(BossEnemyData::kSizeMul),
	_angle(0.0f),
	_alpha(0.0f),
	_fallSpeed(0.0f),
	_drawPosOffset({ BossEnemyData::kDrawPosOffsetX, BossEnemyData::kDrawPosOffsetY }),
	_nowUpdateState(&BossEnemy::FlyUpdate)
{
}

BossEnemy::~BossEnemy()
{
	DeleteGraph(_idleHandle);
	DeleteGraph(_runHandle);
	DeleteGraph(_fallHandle);
}

void BossEnemy::Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int fallHandle)
{
	_map = map;
	_player = player;
	_playerBulletController = playerBulletController;

	_idleHandle = idleHandle;
	assert(_idleHandle >= 0);
	_runHandle = runHandle;
	assert(_runHandle >= 0);
	_fallHandle = fallHandle;
	assert(_fallHandle >= 0);

	_useHandle = _runHandle;
}

void BossEnemy::Update()
{
	(this->*_nowUpdateState)();
}

void BossEnemy::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / BossEnemyData::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * BossEnemyData::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width = BossEnemyData::kGraphWidth;
	int height = BossEnemyData::kGraphHeight;

	if (_alpha != 0.0f)
	{
		float a = _deadFrameCount / (BossEnemyData::kDeathAnimTotalFrame * 2.0f);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));

	}
	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		_sizeMul, _angle,
		_useHandle, true, _isReverseGraphX, false);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG

	DispCol(camera);
	Vector2 offset = camera.lock()->GetDrawOffset();
	int lineLength = 32;
	if (!_isReverseGraphX)
	{
		DrawLine(_pos.x + offset.x, _pos.y + offset.y, _pos.x + offset.x + lineLength, _pos.y + offset.y, 0xff0000);
	}
	else
	{
		DrawLine(static_cast<int>(_pos.x + offset.x),
			static_cast<int>(_pos.y + offset.y),
			static_cast<int>(_pos.x + offset.x - lineLength),
			static_cast<int>(_pos.y + offset.y),
			0xff0000);
	}


#endif // _DEBUG
}

bool BossEnemy::Active(Vector2 pos, bool isReverse)
{
	if (_isActive) return false;

	_isActive = true;
	_pos = pos;
	_isReverseGraphX = isReverse;

	if (_isReverseGraphX) _pos.x += BossEnemyData::kStartPosOffset;
	else				  _pos.x -= BossEnemyData::kStartPosOffset;

	return true;
}

void BossEnemy::DisActive()
{
	if (!_isActive) return;

	_isActive = false;
}

void BossEnemy::OnDamage(int damage, bool isReverseX)
{
	_hitPoint -= damage;

	// 耐久力が0以下になったら非活性化
	if (_hitPoint <= 0)
	{
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &BossEnemy::DeathUpdate;
		_isDead = true;

		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.AddScore(ScoreData::kBossScore);

		float s = _flyFrameCount / 15.0f;
		float fallSpeed = sin(s);
		fallSpeed *= 4.0f;
		_fallSpeed = fallSpeed;

		return;
	}
}
