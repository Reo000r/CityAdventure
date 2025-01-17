#include "Player.h"
#include "GameSceneCamera.h"
#include "Map.h"
#include "MapChip.h"
#include "PlayerBulletController.h"

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
	if (_animFrameCount >= PlayerConstData::kIdleAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// 足元に地面があるか確認
	bool isGround = CheckIsBottom();

	// 足元に地面があるかつジャンプ入力された瞬間なら
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jumpの設定をしてからAir関数を実行してreturn
		_fallSpeed      = PlayerConstData::kJumpForce;
		_animFrameCount = 0;
		_useHandle      = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 足元に地面が無かったら
	if (!isGround)
	{
		// Fallの設定をしてからAir関数を実行してreturn
		_fallSpeed      = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _fallHandle;
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
		_useHandle      = _runHandle;
		_nowUpdateState = &Player::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 発射入力があったら
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// 弾を追加(発射)させる
		// 実際に発射させるかどうかは向こうで決める
		_bulletController->AddBullet(_pos, _isReverseGraphX);
	}
	
	// 弾の更新処理
	_bulletController->Update(_map);
}

void Player::RunUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Run");

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= PlayerConstData::kRunAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// 足元に地面があるか確認
	bool isGround = CheckIsBottom();

	// 足元に地面があるかつジャンプ入力された瞬間なら
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jumpの設定をしてからAir関数を実行してreturn
		_fallSpeed      = PlayerConstData::kJumpForce;
		_animFrameCount = 0;
		_useHandle      = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 足元に地面が無かったら
	if (!isGround)
	{
		// Fallの設定をしてからAir関数を実行してreturn
		_fallSpeed      = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _fallHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 入力がなかったら
	if ((!Input::GetInstance().IsPress(INPUTRIGHT)) &&
		(!Input::GetInstance().IsPress(INPUTLEFT)))
	{
		// 一応Idle関数を実行してreturn
		_vel.x          = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 発射入力があったら
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// 弾を追加(発射)させる
		// 実際に発射させるかどうかは向こうで決める
		_bulletController->AddBullet(_pos, _isReverseGraphX);
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
	_vel = dir * PlayerConstData::kMoveSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;


	// 弾の更新処理
	_bulletController->Update(_map);
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
		if (_animFrameCount >= PlayerConstData::kJumpAnimTotalFrame)
		{
			_animFrameCount = PlayerConstData::kJumpAnimTotalFrame - 1;
		}
	}
	else
	{
		// 下降時

		// アニメーションが1枚のため0にする
		_animFrameCount = 0;
	}

	// 足元に地面があるかつ下に加速しているなら
	if (CheckIsBottom() && _fallSpeed >= 0.0f)
	{
		// 一応Idle関数を実行してreturn
		_vel.x          = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// 発射入力があったら
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// 弾を追加(発射)させる
		// 実際に発射させるかどうかは向こうで決める
		_bulletController->AddBullet(_pos, _isReverseGraphX);
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
	_vel = dir * PlayerConstData::kMoveSpeed;

	// 落下速度の更新
	_fallSpeed += PlayerConstData::kFallSpeed;

	// 地面と接しているかの判定
	// 接しているかどうかでboolを返す
	bool isGround = CheckIsBottom();

	// 上と接しているかの判定
	// つまり上の関数とほぼ同じ
	// bool返ってくるがメンバ変数に保存しているため使わなそう
	CheckIsTop();

	// 移動量に落下速度を加える
	_vel.y += _fallSpeed;

	// 当たり判定を行い、壁にめり込まないように移動ベクトルを修正
	_vel = _map->CheckHitAllMapChip(_pos, _vel, _colSize);

	// 移動
	_pos += _vel;


	// 弾の更新処理
	_bulletController->Update(_map);
}

bool Player::CheckIsBottom() const
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
			std::shared_ptr<MapChip> chip = _map->GetMapChipData(chipX, chipY);

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
			isHit = _map->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
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

	if (isHit)
	{
		// 落下速度をゼロにし、急激な落下を防ぐ
		//_fallSpeed = 0.0f;

		// 後々の雑計算に響くので、y座標の小数点を消し飛ばす
		//_pos.y = floorf(_pos.y);	// ちょうど地面に付く位置に
	}

	return isHit;
}

bool Player::CheckIsTop()
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
			std::shared_ptr<MapChip> chip = _map->GetMapChipData(chipX, chipY);

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
			isHit = _map->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// 落下速度を反転
				_fallSpeed = PlayerConstData::kFallSpeed;
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
		// 1f前にぶつかっていない場合
		if (!_isHitTop)
		{
			//// 落下速度をゼロにし即落下させる
			//_fallSpeed = 0.0f;

			//// 後々の雑計算に響くので、y座標の小数点を消し飛ばす
			//_pos.y = floorf(_pos.y);
		}

	}

	//_isHitTop = isHit;

	return isHit;
}

Player::Player() :
	BoxCollider({ PlayerConstData::kStartPosX, PlayerConstData::kStartPosY }, { PlayerConstData::kColPosOffsetX, PlayerConstData::kColPosOffsetY }, { PlayerConstData::kColWidth, PlayerConstData::kColHeight }),
	_vel(0, 0),
	_drawPosOffset(PlayerConstData::kDrawPosOffsetX, PlayerConstData::kDrawPosOffsetY),
	_bulletController(std::make_shared<PlayerBulletController>()),
	_nowUpdateState(&Player::IdleUpdate),
	_useHandle(0),
	_idleHandle(0),
	_runHandle(0),
	_jumpHandle(0),
	_fallHandle(0),
	_animFrameCount(0),
	_isHitTop(false),
	_isReverseGraphX(false),
	_fallSpeed(0.0f)
{
	//_idleHandle = LoadGraph(L"data/img/player/0/Player0_Idle.png");
	//assert(_idleHandle >= 0);
}

Player::~Player()
{
	DeleteGraph(_idleHandle);
	DeleteGraph(_runHandle);
	DeleteGraph(_jumpHandle);
	DeleteGraph(_fallHandle);
}

void Player::Init(std::shared_ptr<Map> map, int idleHandle, int runHandle, int jumpHandle, int fallHandle)
{
	_map = map;

	_idleHandle = idleHandle;
	assert(_idleHandle >= 0);
	_runHandle = runHandle;
	assert(_runHandle >= 0);
	_jumpHandle = jumpHandle;
	assert(_jumpHandle >= 0);
	_fallHandle = fallHandle;
	assert(_fallHandle >= 0);

	_useHandle = _idleHandle;

	_bulletController->Init();
}

void Player::Update()
{
	(this->*_nowUpdateState)();
}

void Player::Draw(GameSceneCamera camera)
{
	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / PlayerConstData::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * PlayerConstData::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width  = PlayerConstData::kGraphWidth;
	int height = PlayerConstData::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		PlayerConstData::kSizeMul, 0.0,
		_useHandle, true, _isReverseGraphX, false);

	// 弾の描画処理
	_bulletController->Draw(camera);

#ifdef DISP_COLLISION

	DispCol(camera);

	DrawFormatString(96, 96, 0xffffff, L"pos : %.02f/%.02f", _pos.x, _pos.y);
	DrawFormatString(96, 112, 0xffffff, L"vel : %.02f/%.02f", _vel.x, _vel.y);
	DrawFormatString(96, 128, 0xffffff, L"colSize : %d/%d", _colSize.width, _colSize.height);

#endif // DISP_COLLISION
}