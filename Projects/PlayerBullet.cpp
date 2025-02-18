#include "PlayerBullet.h"
#include "Map.h"


PlayerBullet::PlayerBullet(int graphHandle) :
	BoxCollider({ 0.0f, 0.0f }, { PlayerBulletData::kDrawPosOffsetX, PlayerBulletData::kDrawPosOffsetY }, { PlayerBulletData::kColWidth, PlayerBulletData::kColHeight }),
	_graphHandle(graphHandle),
	_animFrameCount(0),
	_isReverseGraphX(false),
	_lifeTimeCount(0),
	_isActive(false),
	_vel({0.0f, 0.0f}), 
	_drawPosOffset({ PlayerBulletData::kDrawPosOffsetX, PlayerBulletData::kDrawPosOffsetY })
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Init()
{
}

void PlayerBullet::Update(std::weak_ptr<Map> map)
{
	// 非活性化状態ならreturn
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
	if (_animFrameCount >= PlayerBulletData::kAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// 移動処理
	if (!_isReverseGraphX)
	{
		// 右
		_vel.x = PlayerBulletData::kMoveSpeed;
	}
	else
	{
		// 左
		_vel.x = -PlayerBulletData::kMoveSpeed;
	}
	_vel.y = 0.0f;

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
}

void PlayerBullet::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// 非活性化状態ならreturn
	if (!_isActive) return;

	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / PlayerBulletData::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + camera.lock()->GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * PlayerBulletData::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width  = PlayerBulletData::kGraphWidth;
	int height = PlayerBulletData::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		PlayerBulletData::kSizeMul, 0.0,
		_graphHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	// デバッグ描画
	DispCol(camera);

#endif // _DEBUG
}

bool PlayerBullet::Active(Vector2 pos, bool isReverse)
{
	// もし活性化済の場合はfalse
	if (_isActive) return false;

	_isActive = true;
	// 左ならtrue、右ならfalse
	_isReverseGraphX = isReverse;

	// 持続時間の設定
	_lifeTimeCount = PlayerBulletData::kLifeTime;

	_pos = pos;
	// 生成位置の補正
	Vector2 offset = PlayerBulletData::kActivePosOffset;
	// x軸の反転処理
	if (_isReverseGraphX)
	{
		offset.x *= -1;
	}
	// 補正
	_pos += offset;

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	return _isActive;
}

void PlayerBullet::DisActive()
{
	// 弾いているはずだが念のため
	if (!_isActive) return;

	_isActive = false;
}