#include "Player.h"
#include "GamePlayScene.h"
#include "GameSceneCamera.h"
#include "GoalObject.h"
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

// namespace�̓w�b�_�ɂ����

void Player::IdleUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Idle");

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= PlayerData::kIdleAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// ���G���Ԃ�����Ȃ猸�炷
	if (_unHitTime > 0)
	{
		_unHitTime--;
	}

	// �����ɒn�ʂ����邩�m�F
	bool isGround = IsHitBottom();

	// �����ɒn�ʂ����邩�W�����v���͂��ꂽ�u�ԂȂ�
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jump�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed = PlayerData::kJumpForce;
		_animFrameCount = 0;
		_useHandle = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����ɒn�ʂ�����������
	if (!isGround)
	{
		// Fall�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _fallHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// ���E���͂���������
	if ((Input::GetInstance().IsPress(INPUTRIGHT)) ||
		(Input::GetInstance().IsPress(INPUTLEFT)))
	{
		// Run�֐������s����return
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Player::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����������ꍇ��
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}

	// ���˓��͂���������
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// �e��ǉ�(����)������
		// ���ۂɔ��˂����邩�ǂ����͌������Ō��߂�
		_bulletController.lock()->AddBullet(_pos, _isReverseGraphX);
	}

	// �S�[������
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// �e�̍X�V����
	_bulletController.lock()->Update(_map);
}

void Player::RunUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Run");

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= PlayerData::kRunAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// ���G���Ԃ�����Ȃ猸�炷
	if (_unHitTime > 0)
	{
		_unHitTime--;
	}

	// �����ɒn�ʂ����邩�m�F
	bool isGround = IsHitBottom();

	// �����ɒn�ʂ����邩�W�����v���͂��ꂽ�u�ԂȂ�
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jump�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed = PlayerData::kJumpForce;
		_animFrameCount = 0;
		_useHandle = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����ɒn�ʂ�����������
	if (!isGround)
	{
		// Fall�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _fallHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// ���͂��Ȃ�������
	if ((!Input::GetInstance().IsPress(INPUTRIGHT)) &&
		(!Input::GetInstance().IsPress(INPUTLEFT)))
	{
		// �ꉞIdle�֐������s����return
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����������ꍇ��
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}

	// ���˓��͂���������
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// �e��ǉ�(����)������
		// ���ۂɔ��˂����邩�ǂ����͌������Ō��߂�
		_bulletController.lock()->AddBullet(_pos, _isReverseGraphX);
	}




	Vector2 dir = { 0.0f, 0.0f };

	// �ړ����������߂�
	if (Input::GetInstance().IsPress(INPUTRIGHT))
	{
		dir.x += 1.0f;
		// �Е��������͂���Ă��Ȃ��ꍇ�͕�����ς���
		if (!Input::GetInstance().IsPress(INPUTLEFT)) _isReverseGraphX = false;
	}
	if (Input::GetInstance().IsPress(INPUTLEFT))
	{
		dir.x += -1.0f;
		// �Е��������͂���Ă��Ȃ��ꍇ�͕�����ς���
		if (!Input::GetInstance().IsPress(INPUTRIGHT)) _isReverseGraphX = true;
	}

	// ���K��
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// ���E�ړ��ʂ��o��
	_vel = dir * PlayerData::kMoveSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;


	// �S�[������
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// �e�̍X�V����
	_bulletController.lock()->Update(_map);
}

void Player::AirUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Air");

	_animFrameCount++;

	// �������㏸���Ă��邩���~���Ă��邩�ɂ���ăA�j���[�V�����𕪂���
	if (_fallSpeed < 0.0f)
	{
		// �㏸��

		// �A�j���[�V�����������������͍Ō�̉摜�������ƕ\�����������̂�
		// �A�j���[�V�����̍��v�t���[�����𒴂�����
		// �ő�l-1�ɂ���(0����n�܂��Ă��邽��)
		if (_animFrameCount >= PlayerData::kJumpAnimTotalFrame)
		{
			_animFrameCount = PlayerData::kJumpAnimTotalFrame - 1;
		}
	}
	else
	{
		// ���~��

		// �A�j���[�V������1���̂���0�ɂ���
		_animFrameCount = 0;
	}

	// ���G���Ԃ�����Ȃ猸�炷
	if (_unHitTime > 0)
	{
		_unHitTime--;
	}

	// �����ɒn�ʂ����邩���ɉ������Ă���Ȃ�
	if (IsHitBottom() && _fallSpeed >= 0.0f)
	{
		// �ꉞIdle�֐������s����return
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����������ꍇ��
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}

	// ���˓��͂���������
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// �e��ǉ�(����)������
		// ���ۂɔ��˂����邩�ǂ����͌������Ō��߂�
		_bulletController.lock()->AddBullet(_pos, _isReverseGraphX);
	}




	Vector2 dir = { 0.0f, 0.0f };

	// �ړ����������߂�
	if (Input::GetInstance().IsPress(INPUTRIGHT))
	{
		dir.x += 1.0f;
		// �Е��������͂���Ă��Ȃ��ꍇ�͕�����ς���
		if (!Input::GetInstance().IsPress(INPUTLEFT)) _isReverseGraphX = false;
	}
	if (Input::GetInstance().IsPress(INPUTLEFT))
	{
		dir.x += -1.0f;
		// �Е��������͂���Ă��Ȃ��ꍇ�͕�����ς���
		if (!Input::GetInstance().IsPress(INPUTRIGHT)) _isReverseGraphX = true;
	}

	// ���K��
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// ���E�ړ��ʂ��o��
	_vel = dir * PlayerData::kMoveSpeed;

	// �������x�̍X�V
	_fallSpeed += PlayerData::kFallSpeed;

	// �n�ʂƐڂ��Ă��邩�̔���
	// �ڂ��Ă��邩�ǂ�����bool��Ԃ�
	bool isGround = IsHitBottom();

	// ��Ɛڂ��Ă��邩�̔���
	// �������Ă����ꍇ�������x�𒲐�
	IsHitTop();

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;


	// �S�[������
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// �e�̍X�V����
	_bulletController.lock()->Update(_map);
}

void Player::DamageUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Damage");

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= PlayerData::kDamageAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// �����ɒn�ʂ����邩���ɉ������Ă���Ȃ�
	if (IsHitBottom() && _fallSpeed >= 0.0f)
	{
		// �ꉞIdle�֐������s����return
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _idleHandle;
		_unHitTime = PlayerData::kUnHitTime;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����������ꍇ��
	if (IsFallOutOfPlayArea())
	{
		(this->*_nowUpdateState)();
		return;
	}



	Vector2 dir = { 0.0f, 0.0f };

	if (!_isReverseGraphX)
	{
		// �E�������Ă���Ȃ獶�Ƀm�b�N�o�b�N
		dir.x -= 1.0f;
	}
	else
	{
		// ���������Ă���Ȃ�E�Ƀm�b�N�o�b�N
		dir.x += 1.0f;
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// ���E�ړ��ʂ��o��
	_vel = dir * PlayerData::kMoveSpeed * 0.5f;

	// �������x�̍X�V
	_fallSpeed += PlayerData::kFallSpeed;

	// �n�ʂƐڂ��Ă��邩�̔���
	// �ڂ��Ă��邩�ǂ�����bool��Ԃ�
	bool isGround = IsHitBottom();

	// ��Ɛڂ��Ă��邩�̔���
	// �������Ă����ꍇ�������x�𒲐�
	IsHitTop();

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;


	// �S�[������
	if (IsHitGoal())
	{
		_isGoal = true;
	}

	// �e�̍X�V����
	_bulletController.lock()->Update(_map);
}

void Player::DeathUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Death");

	if (_isDead) return;

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����
	// �V�[���`�F���W��������
	if (_animFrameCount >= PlayerData::kDeathAnimTotalFrame)
	{
		_animFrameCount = PlayerData::kDeathAnimTotalFrame - 1;
		_isDead = true;
		return;
	}



	Vector2 dir = { 0.0f, 0.0f };

	if (!_isReverseGraphX)
	{
		// �E�������Ă���Ȃ獶�Ƀm�b�N�o�b�N
		dir.x -= 1.0f;
	}
	else
	{
		// ���������Ă���Ȃ�E�Ƀm�b�N�o�b�N
		dir.x += 1.0f;
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// ���E�ړ��ʂ��o��
	_vel = dir * PlayerData::kMoveSpeed * 0.25f;

	// �������x�̍X�V
	_fallSpeed += PlayerData::kFallSpeed * 0.5f;

	// �n�ʂƐڂ��Ă��邩�̔���
	// �ڂ��Ă��邩�ǂ�����bool��Ԃ�
	bool isGround = IsHitBottom();

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;


	// �e�̍X�V����
	//_bulletController.lock()->Update(_map);
}

void Player::FallDeathUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : FallDeath");

	if (_isDead) return;

	// �������͑����ŗǂ��̂ł�
	_isDead = true;
	//return;

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����
	// �V�[���`�F���W��������
	if (_animFrameCount >= PlayerData::kDeathAnimTotalFrame)
	{
		_animFrameCount = PlayerData::kDeathAnimTotalFrame - 1;
		_isDead = true;
		return;
	}


	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// �������x�̍X�V
	_fallSpeed += PlayerData::kFallSpeed * 0.5f;

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;
}

bool Player::IsHitBottom() const
{
	// 1�h�b�g���ɂ��炵�A�������Ă���Βn�ʂɂ��Ă���
	Vector2 checkPos = _pos;
	checkPos.y += 1.0f;

	// �S�}�b�v�`�b�v�������蔻����s��
	bool isHit = false;
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// ��O����
			if (chip == nullptr)
			{
				assert(false && "�͈͊O�ǂݎ��");
				continue;
			}

			// �����蔻��������Ȃ��}�b�v�`�b�v�Ȃ画�菈�����΂�
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// �����蔻����s��
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

	if (isHit)
	{
		// �������x���[���ɂ��A�}���ȗ�����h��
		//_fallSpeed = 0.0f;

		// ��X�̎G�v�Z�ɋ����̂ŁAy���W�̏����_��������΂�
		//_pos.y = floorf(_pos.y);	// ���傤�ǒn�ʂɕt���ʒu��
	}

	return isHit;
}

bool Player::IsHitTop()
{
	// 1�h�b�g��ɂ��炵�A�������Ă���ΓV��ɒ����Ă���
	Vector2 checkPos = _pos;
	checkPos.y -= 1.0f;

	// �S�}�b�v�`�b�v�������蔻����s��
	bool isHit = false;
	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			std::shared_ptr<MapChip> chip = _map.lock()->GetMapChipData(chipX, chipY);

			// ��O����
			if (chip == nullptr)
			{
				assert(false && "�͈͊O�ǂݎ��");
				continue;
			}

			// �����蔻��������Ȃ��}�b�v�`�b�v�Ȃ画�菈�����΂�
			int chipNo = chip->GetMapChipNum();
			if (Game::CheckEmptyMapChip(chipNo)) continue;

			// �����蔻����s��
			isHit = _map.lock()->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// �������x�𔽓]
				_fallSpeed = PlayerData::kFallSpeed;
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
		// 1f�O�ɂԂ����Ă��Ȃ��ꍇ
		if (!_isHitTop)
		{
			//// �������x���[���ɂ�������������
			//_fallSpeed = 0.0f;

			//// ��X�̎G�v�Z�ɋ����̂ŁAy���W�̏����_��������΂�
			//_pos.y = floorf(_pos.y);
		}

	}

	//_isHitTop = isHit;

	return isHit;
}

bool Player::IsHitGoal() const
{
	// ����(�G)�̋�`�����擾
	Game::Rect rect = GetRect();
	// �v���C���[�̓����蔻����擾
	Game::Rect playerRect = _goal.lock()->GetRect();

	// �e�Ɠ����蔻��
	bool isHit = !(
		(rect.top >= playerRect.bottom || rect.bottom <= playerRect.top) ||
		(rect.left >= playerRect.right || rect.right <= playerRect.left));
	
	return isHit;
}

bool Player::IsFallOutOfPlayArea()
{
	// ��O�֗��������ꍇ�͎��S
	if (_pos.y >= PlayerData::kDeadHeight)
	{
		// �ݒ�����Ă���return
		_animFrameCount = 0;
		_useHandle = _deathHandle;
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
	//_idleHandle = LoadGraph(L"data/img/player/0/Player0_Idle.png");
	//assert(_idleHandle >= 0);
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
	//_gamePlayScene = gamePlayScene;
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
	// ���G���Ԓ��͓_�ł�����
	// 2f���ɐ؂�ւ���
	int blinkTime = 8;
	if (!(_unHitTime % blinkTime * 2 >= blinkTime))
	{
		// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
		int animNo = _animFrameCount / PlayerData::kSingleAnimFrame;
		// ���S���̓A�j���[�V�������x��ύX
		if (_nowUpdateState == &Player::DeathUpdate)
		{
			animNo = _animFrameCount / PlayerData::kDeathAnimFrame;
		}

		Vector2 cameraPos = camera.lock()->GetDrawOffset();
		// �\���ʒu(���S)
		int x = static_cast<int>(_pos.x + _drawPosOffset.x + cameraPos.x);
		int y = static_cast<int>(_pos.y + _drawPosOffset.y + cameraPos.y);

		// �؂�o���J�n�ʒu
		int cutX = animNo * PlayerData::kGraphWidth;
		int cutY = 0;

		// �؂�o����
		int width = PlayerData::kGraphWidth;
		int height = PlayerData::kGraphHeight;

		DrawRectRotaGraph(
			x, y,
			cutX, cutY,
			width, height,
			PlayerData::kSizeMul, 0.0,
			_useHandle, true, _isReverseGraphX, false);
	}

	// �e�̕`�揈��
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
	// ��e�������S���Ȃ�return
	if ((_nowUpdateState == &Player::DamageUpdate) ||
		(_nowUpdateState == &Player::DeathUpdate)) return;
	// ���G���Ԓ��Ȃ�return
	if (_unHitTime > 0) return;

	_hitPoint -= damage;

	// �ϋv�͂�0�ȉ��ɂȂ����玀�S�X�e�[�g��
	if (_hitPoint <= 0)
	{
		// �ݒ�����Ă���return
		_fallSpeed = PlayerData::kJumpForce * 0.25f;
		_animFrameCount = 0;
		_useHandle = _deathHandle;
		_isReverseGraphX = isReverseX;
		_nowUpdateState = &Player::DeathUpdate;
		return;
	}
	// �����łȂ��Ȃ��e�X�e�[�g��
	else
	{
		// �ݒ�����Ă���return
		_fallSpeed = PlayerData::kJumpForce * 0.5f;
		_animFrameCount = 0;
		//_useHandle = _damageHandle;
		_isReverseGraphX = isReverseX;
		_nowUpdateState = &Player::DamageUpdate;
		return;
	}
}