#include "Player.h"
#include "GamePlayScene.h"
#include "GameSceneCamera.h"
#include "GoalObject.h"
#include "Map.h"
#include "MapChip.h"
#include "PlayerBulletController.h"
#include "scoreController.h"

#include "Input.h"
#include "Vector2.h"

#include <DxLib.h>
#include <cassert>
#include <memory>

#ifdef _DEBUG
#define DISP_COLLISION
#endif // _DEBUG

// namespaceはヘッダにあるよ

void Player::IdleUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Idle");

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= PlayerData::kIdleAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// 無敵時間があるなら減らす
	if (_unHitTime > 0)
	{
		_unHitTime--;
	}

	// 足元に地面があるか確認
	bool isGround = IsHitBottom();

	// 足元に地面があるかつジャンプ入力された瞬間なら
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jumpの設定をしてからAir関数を実行してreturn
		_fallSpeed = PlayerData::kJumpForce;
		_animFrameCount = 0;
		_useHandle = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 足元に地面が無かったら
	if (!isGround)
	{
		// Fallの設定をしてからAir関数を実行してreturn
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _fallHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 左右入力があったら
	if ((Input::GetInstance().IsPress(INPUTRIGHT)) ||
		(Input::GetInstance().IsPress(INPUTLEFT)))
	{
		// Run関数を実行してreturn
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Player::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 落下死した場合は
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}

	// 発射入力があったら
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// 弾を追加(発射)させる
		// 実際に発射させるかどうかは向こうで決める
		_bulletController.lock()->AddBullet(_pos, _isReverseGraphX);
	}

	// ゴール判定
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// 弾の更新処理
	_bulletController.lock()->Update(_map);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.SetPlayerHitpoint(_hitPoint);
}

void Player::RunUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Run");

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= PlayerData::kRunAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// 無敵時間があるなら減らす
	if (_unHitTime > 0)
	{
		_unHitTime--;
	}

	// 足元に地面があるか確認
	bool isGround = IsHitBottom();

	// 足元に地面があるかつジャンプ入力された瞬間なら
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jumpの設定をしてからAir関数を実行してreturn
		_fallSpeed = PlayerData::kJumpForce;
		_animFrameCount = 0;
		_useHandle = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 足元に地面が無かったら
	if (!isGround)
	{
		// Fallの設定をしてからAir関数を実行してreturn
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _fallHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 入力がなかったら
	if ((!Input::GetInstance().IsPress(INPUTRIGHT)) &&
		(!Input::GetInstance().IsPress(INPUTLEFT)))
	{
		// 一応Idle関数を実行してreturn
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 落下死した場合は
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}

	// 発射入力があったら
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// 弾を追加(発射)させる
		// 実際に発射させるかどうかは向こうで決める
		_bulletController.lock()->AddBullet(_pos, _isReverseGraphX);
	}




	Vector2 dir = { 0.0f, 0.0f };

	// 移動方向を求める
	if (Input::GetInstance().IsPress(INPUTRIGHT))
	{
		dir.x += 1.0f;
		// 片方しか入力されていない場合は方向を変える
		if (!Input::GetInstance().IsPress(INPUTLEFT)) _isReverseGraphX = false;
	}
	if (Input::GetInstance().IsPress(INPUTLEFT))
	{
		dir.x += -1.0f;
		// 片方しか入力されていない場合は方向を変える
		if (!Input::GetInstance().IsPress(INPUTRIGHT)) _isReverseGraphX = true;
	}

	// 正規化
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 左右移動量を出す
	_vel = dir * PlayerData::kMoveSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;


	// ゴール判定
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// 弾の更新処理
	_bulletController.lock()->Update(_map);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.SetPlayerHitpoint(_hitPoint);
}

void Player::AirUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Air");

	_animFrameCount++;

	// 自分が上昇しているか下降しているかによってアニメーションを分ける
	if (_fallSpeed < 0.0f)
	{
		// 上昇時

		// アニメーションを一周させた後は最後の画像をずっと表示させたいので
		// アニメーションの合計フレーム数を超えたら
		// 最大値-1にする(0から始まっているため)
		if (_animFrameCount >= PlayerData::kJumpAnimTotalFrame)
		{
			_animFrameCount = PlayerData::kJumpAnimTotalFrame - 1;
		}
	}
	else
	{
		// 下降時

		// アニメーションが1枚のため0にする
		_animFrameCount = 0;
	}

	// 無敵時間があるなら減らす
	if (_unHitTime > 0)
	{
		_unHitTime--;
	}

	// 足元に地面があるかつ下に加速しているなら
	if (IsHitBottom() && _fallSpeed >= 0.0f)
	{
		// Idle関数を実行してreturn
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 落下死した場合は
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}

	// 発射入力があったら
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// 弾を追加(発射)させる
		// 実際に発射させるかどうかは向こうで決める
		_bulletController.lock()->AddBullet(_pos, _isReverseGraphX);
	}




	Vector2 dir = { 0.0f, 0.0f };

	// 移動方向を求める
	if (Input::GetInstance().IsPress(INPUTRIGHT))
	{
		dir.x += 1.0f;
		// 片方しか入力されていない場合は方向を変える
		if (!Input::GetInstance().IsPress(INPUTLEFT)) _isReverseGraphX = false;
	}
	if (Input::GetInstance().IsPress(INPUTLEFT))
	{
		dir.x += -1.0f;
		// 片方しか入力されていない場合は方向を変える
		if (!Input::GetInstance().IsPress(INPUTRIGHT)) _isReverseGraphX = true;
	}

	// 正規化
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 左右移動量を出す
	_vel = dir * PlayerData::kMoveSpeed;

	// 落下速度の更新
	_fallSpeed += PlayerData::kFallSpeed;

	// 地面と接しているかの判定
	// 接しているかどうかでboolを返す
	bool isGround = IsHitBottom();

	// 上と接しているかの判定
	// 当たっていた場合落下速度を調整
	IsHitTop();

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;


	// ゴール判定
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// 弾の更新処理
	_bulletController.lock()->Update(_map);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.SetPlayerHitpoint(_hitPoint);
}

void Player::DamageUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Damage");

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= PlayerData::kDamageAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// 足元に地面があるかつ下に加速しているなら
	if (IsHitBottom() && _fallSpeed >= 0.0f)
	{
		// 一応Idle関数を実行してreturn
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_unHitTime = PlayerData::kUnHitTime;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 落下死した場合は
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}



	Vector2 dir = { 0.0f, 0.0f };

	if (!_isReverseGraphX)
	{
		// 右を向いているなら左にノックバック
		dir.x -= 1.0f;
	}
	else
	{
		// 左を向いているなら右にノックバック
		dir.x += 1.0f;
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 左右移動量を出す
	_vel = dir * PlayerData::kMoveSpeed * 0.5f;

	// 落下速度の更新
	_fallSpeed += PlayerData::kFallSpeed;

	// 地面と接しているかの判定
	// 接しているかどうかでboolを返す
	bool isGround = IsHitBottom();

	// 上と接しているかの判定
	// 当たっていた場合落下速度を調整
	IsHitTop();

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;


	// ゴール判定
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// 弾の更新処理
	_bulletController.lock()->Update(_map);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.SetPlayerHitpoint(_hitPoint);
}

void Player::DeathUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Death");

	if (_isDead) return;

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら
	// シーンチェンジさせたい
	if (_animFrameCount >= PlayerData::kDeathAnimTotalFrame)
	{
		_animFrameCount = PlayerData::kDeathAnimTotalFrame - 1;
		_isDead = true;
		return;
	}



	Vector2 dir = { 0.0f, 0.0f };

	if (!_isReverseGraphX)
	{
		// 右を向いているなら左にノックバック
		dir.x -= 1.0f;
	}
	else
	{
		// 左を向いているなら右にノックバック
		dir.x += 1.0f;
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 左右移動量を出す
	_vel = dir * PlayerData::kMoveSpeed * 0.25f;

	// 落下速度の更新
	_fallSpeed += PlayerData::kFallSpeed * 0.5f;

	// 地面と接しているかの判定
	// 接しているかどうかでboolを返す
	bool isGround = IsHitBottom();

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;


	// 弾の更新処理
	_bulletController.lock()->Update(_map);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.SetPlayerHitpoint(_hitPoint);
}

void Player::FallDeathUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : FallDeath");

	if (_isDead) return;

	// 落下死は即死
	_isDead = true;

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// 落下速度の更新
	_fallSpeed += PlayerData::kFallSpeed * 0.5f;

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;
}

bool Player::IsHitBottom() const
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

bool Player::IsHitTop()
{
	// 1ドット上にずらし、当たっていれば天井に着いている
	Vector2 checkPos = _pos;
	checkPos.y -= 1.0f;

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
				// 落下速度を反転
				_fallSpeed = PlayerData::kFallSpeed;
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

bool Player::IsHitGoal() const
{
	if (!_goal.lock()->IsActive()) return false;

	// 自分の矩形情報を取得
	Game::Rect rect = GetRect();
	// 当たり判定を取得
	Game::Rect goalRect = _goal.lock()->GetRect();

	// 弾と当たり判定
	bool isHit = !(
		(rect.top >= goalRect.bottom || rect.bottom <= goalRect.top) ||
		(rect.left >= goalRect.right || rect.right  <= goalRect.left));
	
	return isHit;
}

bool Player::IsFallOutOfPlayArea()
{
	// 場外へ落下した場合は死亡
	if (_pos.y >= PlayerData::kDeadHeight)
	{
		// 設定をしてからreturn
		_hitPoint = 0;
		_animFrameCount = 0;
		_useHandle = _deathHandle;

		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetPlayerHitpoint(_hitPoint);
		
		_nowUpdateState = &Player::FallDeathUpdate;
		return true;
	}

	return false;
}

Player::Player() :
	BoxCollider({ PlayerData::kStartPosX, PlayerData::kStartPosY }, { PlayerData::kColPosOffsetX, PlayerData::kColPosOffsetY }, { PlayerData::kColWidth, PlayerData::kColHeight }),
	_vel(0, 0),
	_drawPosOffset(PlayerData::kDrawPosOffsetX, PlayerData::kDrawPosOffsetY),
	_nowUpdateState(&Player::IdleUpdate),
	_useHandle(0),
	_idleHandle(0),
	_runHandle(0),
	_jumpHandle(0),
	_fallHandle(0),
	_deathHandle(0),
	_animFrameCount(0),
	_hitPoint(PlayerData::kHitPoint),
	_unHitTime(0),
	_isActive(false),
	_isDead(false),
	_isGoal(false),
	_isHitTop(false),
	_isReverseGraphX(false),
	_fallSpeed(0.0f)
{
}

Player::~Player()
{
	DeleteGraph(_idleHandle);
	DeleteGraph(_runHandle);
	DeleteGraph(_jumpHandle);
	DeleteGraph(_fallHandle);
	DeleteGraph(_deathHandle);
}

void Player::Init(std::weak_ptr<GoalObject> goal, std::weak_ptr<Map> map, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int jumpHandle, int fallHandle, int deathHandle)
{
	_goal = goal;
	_map = map;
	_bulletController = playerBulletController;

	_idleHandle = idleHandle;
	assert(_idleHandle >= 0);
	_runHandle = runHandle;
	assert(_runHandle >= 0);
	_jumpHandle = jumpHandle;
	assert(_jumpHandle >= 0);
	_fallHandle = fallHandle;
	assert(_fallHandle >= 0);
	_deathHandle = deathHandle;
	assert(_deathHandle >= 0);


	_useHandle = _idleHandle;

	_isActive = true;

	_bulletController.lock()->Init();
}

void Player::Update()
{
	(this->*_nowUpdateState)();
}

void Player::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// 無敵時間中は点滅させる
	int blinkTime = 8;
	if (!(_unHitTime % blinkTime * 2 >= blinkTime))
	{
		// グラフィックの切り出し位置(X座標)を計算する
		int animNo = _animFrameCount / PlayerData::kSingleAnimFrame;
		// 死亡時はアニメーション速度を変更
		if (_nowUpdateState == &Player::DeathUpdate)
		{
			animNo = _animFrameCount / PlayerData::kDeathAnimFrame;
		}

		Vector2 cameraPos = camera.lock()->GetDrawOffset();
		// 表示位置(中心)
		int x = static_cast<int>(_pos.x + _drawPosOffset.x + cameraPos.x);
		int y = static_cast<int>(_pos.y + _drawPosOffset.y + cameraPos.y);

		// 切り出し開始位置
		int cutX = animNo * PlayerData::kGraphWidth;
		int cutY = 0;

		// 切り出し量
		int width = PlayerData::kGraphWidth;
		int height = PlayerData::kGraphHeight;

		DrawRectRotaGraph(
			x, y,
			cutX, cutY,
			width, height,
			PlayerData::kSizeMul, 0.0,
			_useHandle, true, _isReverseGraphX, false);
	}

	// 弾の描画処理
	_bulletController.lock()->Draw(camera);

#ifdef DISP_COLLISION

	DispCol(camera);

	Vector2 cameraPos = camera.lock()->GetDrawOffset();
	int constY = 96;
	int addY   = 16;
	int drawY = constY;
	DrawFormatString(0, drawY, 0xffffff, L"pos : %.02f/%.02f", _pos.x, _pos.y);
	drawY += addY;
	DrawFormatString(0, drawY, 0xffffff, L"vel : %.02f/%.02f", _vel.x, _vel.y);
	drawY += addY;
	DrawFormatString(0, drawY, 0xffffff, L"colSize : %d/%d", _colSize.width, _colSize.height);
	drawY += addY;
	DrawFormatString(0, drawY, 0xffffff, L"CameraPos : %f/%f", cameraPos.x, cameraPos.y);
	drawY += addY;
	DrawFormatString(0, drawY, 0xffffff, L"isGoal : %d", static_cast<int>(_isGoal));

#endif // DISP_COLLISION
}

void Player::OnDamage(int damage, bool isReverseX)
{
	// 被弾中か死亡中ならreturn
	if ((_nowUpdateState == &Player::DamageUpdate) ||
		(_nowUpdateState == &Player::DeathUpdate)) return;
	// 無敵時間中ならreturn
	if (_unHitTime > 0) return;

	_hitPoint -= damage;

	// 耐久力が0以下になったら死亡ステートへ
	if (_hitPoint <= 0)
	{
		// 設定をしてからreturn
		_fallSpeed = PlayerData::kJumpForce * 0.25f;
		_animFrameCount = 0;
		_useHandle = _deathHandle;
		_isReverseGraphX = isReverseX;
		_nowUpdateState = &Player::DeathUpdate;
		return;
	}
	// そうでないなら被弾ステートへ
	else
	{
		// 設定をしてからreturn
		_fallSpeed = PlayerData::kJumpForce * 0.5f;
		_animFrameCount = 0;
		_isReverseGraphX = isReverseX;
		_nowUpdateState = &Player::DamageUpdate;
		return;
	}
}