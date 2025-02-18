#include "Enemy01.h"
#include "Map.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerBulletController.h"
#include "ScoreController.h"

#include "Input.h"

#include <cassert>

void Enemy01::IdleUpdate()
{
	// プレイヤーの一定範囲に入るまで待機状態
	// 入っていたら走り始める
	if(CheckPlayerDistance())
	{
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Enemy01::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= Enemy01Data::kIdleAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	_blinkFrameCount--;
	if (_blinkFrameCount <= 0) _blinkFrameCount = 0;

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

	// プレイヤーと当たり判定を行う
	IsPlayerHit();
}

void Enemy01::RunUpdate()
{
	// プレイヤーの一定範囲に入っていなかったら待機状態へ
	if (!CheckPlayerDistance())
	{
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_nowUpdateState = &Enemy01::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= Enemy01Data::kRunAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	_blinkFrameCount--;
	if (_blinkFrameCount <= 0) _blinkFrameCount = 0;

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

	// 足元に地面があるか確認
	bool isGround = IsHitBottom();

	// 足元に地面が無かったら
	if (!isGround)
	{
		// Fallの設定をしてからAir関数を実行してreturn
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _fallHandle;
		_nowUpdateState = &Enemy01::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}


	Vector2 dir = { 0.0f, 0.0f };

	// 向いている方向から移動方向を求める
	if (!_isReverseGraphX)
	{
		// 右
		dir.x += 1.0f;
	}
	else
	{
		// 左
		dir.x -= 1.0f;
	}

	// 正規化
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 左右移動量を出す
	_vel = dir * Enemy01Data::kMoveSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;

	// プレイヤーと当たり判定を行う
	IsPlayerHit();

	// 左右に壁があるかを判定
	if (IsRightAndLeft())
	{
		// あった場合は反転
		_isReverseGraphX = !_isReverseGraphX;
	}


}

void Enemy01::AirUpdate()
{
	// プレイヤーの一定範囲に入るまでUpdateを止める
	Vector2 playerPos = _player.lock()->GetPos();
	if ((playerPos.x + Game::kScreenCenterWidth + Enemy01Data::kActivationRange <= _pos.x) ||
		(playerPos.x - Game::kScreenCenterWidth - Enemy01Data::kActivationRange >= _pos.x)) return;

	_animFrameCount++;
	
	// 下降時
	// アニメーションが1枚想定
	_animFrameCount = 0;

	_blinkFrameCount--;
	if (_blinkFrameCount <= 0) _blinkFrameCount = 0;

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

	// 足元に地面があるかつ下に加速しているなら
	if (IsHitBottom() && _fallSpeed >= 0.0f)
	{
		// Run関数を実行してreturn
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Enemy01::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}



	Vector2 dir = { 0.0f, 0.0f };

	// 向いている方向から移動方向を求める
	if (!_isReverseGraphX)
	{
		// 右
		dir.x += 1.0f;
	}
	else
	{
		// 左
		dir.x -= 1.0f;
	}

	// 正規化
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 左右移動量を出す
	_vel = dir * Enemy01Data::kMoveSpeed;

	// 落下速度の更新
	_fallSpeed += Enemy01Data::kFallSpeed;

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;

	// プレイヤーと当たり判定を行う
	IsPlayerHit();

	// 左右に壁があるかを判定
	if (IsRightAndLeft())
	{
		// あった場合は反転
		_isReverseGraphX = !_isReverseGraphX;
	}
}

void Enemy01::DeathUpdate()
{
	_deadFrameCount++;

	float maxDeadFrame = 60.0f;
	_sizeMul -= 1 * Enemy01Data::kSizeMul / maxDeadFrame;
	_angle += (_deadFrameCount / maxDeadFrame);

	if (_deadFrameCount > maxDeadFrame)
	{
		DisActive();
		return;
	}
}

bool Enemy01::IsHitBottom() const
{
	// 1ドット下にずらし、当たっていれば地面についている
	Vector2 checkPos = _pos;
	checkPos.y += 1.0f;

	// 全マップチップ分当たり判定を行う
	bool isHit = false;
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// 例外処理
			if (chip == nullptr)
			{
				assert(false && "範囲外読み取り");
				continue;
			}

			// 当たり判定を持たないマップチップなら判定処理を飛ばす
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// 当たり判定を行う
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}

	return isHit;
}

bool Enemy01::IsRightAndLeft()
{
	// 右

	// 1ドット右にずらし、当たっていれば壁に触れている
	Vector2 checkPos = _pos;
	checkPos.x += 1.0f;

	// 全マップチップ分当たり判定を行う
	bool isHit = false;
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// 例外処理
			if (chip == nullptr)
			{
				assert(false && "範囲外読み取り");
				continue;
			}

			// 当たり判定を持たないマップチップなら判定処理を飛ばす
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// 当たり判定を行う
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// 当たっていれば処理を飛ばす
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}

	if (isHit)
	{
		return isHit;
	}

	// 左

	// 1ドット左にずらし、当たっていれば壁に触れている
	checkPos = _pos;
	checkPos.x -= 1.0f;

	// 全マップチップ分当たり判定を行う
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// 例外処理
			if (chip == nullptr)
			{
				assert(false && "範囲外読み取り");
				continue;
			}

			// 当たり判定を持たないマップチップなら判定処理を飛ばす
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// 当たり判定を行う
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// 左へ
				//_isReverseGraphX = true;
				break;
			}
		}
		if (isHit)
		{
			break;
		}
	}

	return isHit;
}

bool Enemy01::IsPlayerHit()
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
		_player.lock()->OnDamage(Enemy01Data::kAddDamageAmount, isReverseX);
		return true;
	}

	return false;
}

Enemy01::Enemy01(Vector2 pos) :
	Enemy(pos, { Enemy01Data::kColPosOffsetX, Enemy01Data::kColPosOffsetY }, { Enemy01Data::kColWidth,Enemy01Data::kColHeight }, Enemy01Data::kHitPoint),
	_useHandle(0),
	_idleHandle(0),
	_runHandle(0),
	_fallHandle(0),
	_animFrameCount(0),
	_isHitTop(false),
	_isDead(false),
	_deadFrameCount(0),
	_sizeMul(Enemy01Data::kSizeMul),
	_angle(0),
	_fallSpeed(0.0f),
	_drawPosOffset({ Enemy01Data::kDrawPosOffsetX, Enemy01Data::kDrawPosOffsetY }),
	_nowUpdateState(&Enemy01::IdleUpdate)
{
}

Enemy01::~Enemy01()
{
	DeleteGraph(_idleHandle);
	DeleteGraph(_runHandle);
	DeleteGraph(_fallHandle);
}

void Enemy01::Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int fallHandle)
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

	_useHandle = _idleHandle;
}

void Enemy01::Update()
{
	(this->*_nowUpdateState)();
}

void Enemy01::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	int blinkTime = 4;
	if (_blinkFrameCount % blinkTime * 2 >= blinkTime) return;

	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / Enemy01Data::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * Enemy01Data::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width  = Enemy01Data::kGraphWidth;
	int height = Enemy01Data::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		_sizeMul, _angle,
		_useHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	// デバッグ描画
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

bool Enemy01::Active(Vector2 pos, bool isReverse)
{
	if (_isActive) return false;

	_isActive = true;
	_pos = pos;
	_isReverseGraphX = isReverse;

	return true;
}

void Enemy01::DisActive()
{
	if (!_isActive) return;

	_isActive = false;
}

void Enemy01::OnDamage(int damage, bool isReverseX)
{
	_hitPoint -= damage;

	// 耐久力が0以下になったら非活性化
	if (_hitPoint <= 0)
	{
		_blinkFrameCount = 0;
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Enemy01::DeathUpdate;
		_isDead = true;

		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.AddScore(ScoreData::kEnemy01Score);

		return;
	}
	else
	{
		if (_blinkFrameCount <= 0) _blinkFrameCount = 8;
	}
}
