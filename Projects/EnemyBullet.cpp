#include "EnemyBullet.h"
#include "Map.h"
#include "Player.h"


bool EnemyBullet::IsHitPlayer()
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
		_player.lock()->OnDamage(EnemyBulletData::kAddDamageAmount, isReverseX);
		return true;
	}

	return false;
}

EnemyBullet::EnemyBullet(int graphHandle) :
	BoxCollider({ 0.0f, 0.0f }, { EnemyBulletData::kDrawPosOffsetX, EnemyBulletData::kDrawPosOffsetY }, { EnemyBulletData::kColWidth, EnemyBulletData::kColHeight }),
	_graphHandle(graphHandle),
	_animFrameCount(0),
	_isReverseGraphX(false),
	_lifeTimeCount(0),
	_isActive(false),
	_vel({ 0.0f, 0.0f }),
	_dir({ 0.0f, 0.0f }),
	_drawPosOffset({ EnemyBulletData::kDrawPosOffsetX, EnemyBulletData::kDrawPosOffsetY })
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Init(std::weak_ptr<Player> player)
{
	_player = player;
}

void EnemyBullet::Update(std::weak_ptr<Map> map)
{
	// Controller側で弾いているはずだが念のため
	if (!_isActive) return;

	_lifeTimeCount--;
	// 持続時間を超えていたら
	if (_lifeTimeCount <= 0 ||
		_pos.x <= 0.0f)
	{
		//非活性化
		DisActive();
		return;
	}

	_animFrameCount++;
	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= EnemyBulletData::kAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	_vel = _dir * EnemyBulletData::kMoveSpeed;

	// 当たり判定
	Vector2 returnVel = map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動ベクトルが修正された場合は当たっている
	if (_vel.x != returnVel.x || _vel.y != returnVel.y)
	{
		// 当たっていた場合は非活性化
		DisActive();
		return;
	}

	// 移動
	_pos += _vel;
	
	if (IsHitPlayer())
	{
		DisActive();
	}
}

void EnemyBullet::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// Controller側で弾いているはずだが念のため
	if (!_isActive) return;

	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / EnemyBulletData::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + camera.lock()->GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * EnemyBulletData::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width = EnemyBulletData::kGraphWidth;
	int height = EnemyBulletData::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		EnemyBulletData::kSizeMul, 0.0,
		_graphHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	DispCol(camera);

#endif // _DEBUG
}

bool EnemyBullet::Active(Vector2 pos, bool isReverse)
{
	// もし活性化済の場合はfalse
	if (_isActive) return false;

	_isActive = true;
	// 左ならtrue、右ならfalse
	_isReverseGraphX = isReverse;

	// 持続時間の設定
	_lifeTimeCount = EnemyBulletData::kLifeTime;

	_pos = pos;
	
	Vector2 playerPos = _player.lock()->GetPos();

	_dir = playerPos - _pos;
	// Vector2改修しよう...
	_dir = _dir.Normalized(_dir);

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	return _isActive;
}

void EnemyBullet::DisActive()
{
	// 弾いているはずだが念のため
	if (!_isActive) return;

	_isActive = false;
}