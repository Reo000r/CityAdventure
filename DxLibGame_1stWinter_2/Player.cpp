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

// namespace�̓w�b�_�ɂ����

void Player::IdleUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Idle");

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= PlayerConstData::kIdleAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// �����ɒn�ʂ����邩�m�F
	bool isGround = CheckIsBottom();

	// �����ɒn�ʂ����邩�W�����v���͂��ꂽ�u�ԂȂ�
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jump�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed      = PlayerConstData::kJumpForce;
		_animFrameCount = 0;
		_useHandle      = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����ɒn�ʂ�����������
	if (!isGround)
	{
		// Fall�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed      = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _fallHandle;
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
		_useHandle      = _runHandle;
		_nowUpdateState = &Player::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// ���˓��͂���������
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// �e��ǉ�(����)������
		// ���ۂɔ��˂����邩�ǂ����͌������Ō��߂�
		_bulletController->AddBullet(_pos, _isReverseGraphX);
	}
	
	// �e�̍X�V����
	_bulletController->Update(_map);
}

void Player::RunUpdate()
{
	DrawFormatString(96, 80, 0xffffff, L"State : Run");

	_animFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= PlayerConstData::kRunAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// �����ɒn�ʂ����邩�m�F
	bool isGround = CheckIsBottom();

	// �����ɒn�ʂ����邩�W�����v���͂��ꂽ�u�ԂȂ�
	if (isGround && Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		// Jump�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed      = PlayerConstData::kJumpForce;
		_animFrameCount = 0;
		_useHandle      = _jumpHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// �����ɒn�ʂ�����������
	if (!isGround)
	{
		// Fall�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed      = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _fallHandle;
		_nowUpdateState = &Player::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// ���͂��Ȃ�������
	if ((!Input::GetInstance().IsPress(INPUTRIGHT)) &&
		(!Input::GetInstance().IsPress(INPUTLEFT)))
	{
		// �ꉞIdle�֐������s����return
		_vel.x          = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// ���˓��͂���������
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// �e��ǉ�(����)������
		// ���ۂɔ��˂����邩�ǂ����͌������Ō��߂�
		_bulletController->AddBullet(_pos, _isReverseGraphX);
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
	_vel = dir * PlayerConstData::kMoveSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;


	// �e�̍X�V����
	_bulletController->Update(_map);
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
		if (_animFrameCount >= PlayerConstData::kJumpAnimTotalFrame)
		{
			_animFrameCount = PlayerConstData::kJumpAnimTotalFrame - 1;
		}
	}
	else
	{
		// ���~��

		// �A�j���[�V������1���̂���0�ɂ���
		_animFrameCount = 0;
	}

	// �����ɒn�ʂ����邩���ɉ������Ă���Ȃ�
	if (CheckIsBottom() && _fallSpeed >= 0.0f)
	{
		// �ꉞIdle�֐������s����return
		_vel.x          = 0.0f;
		_animFrameCount = 0;
		_useHandle      = _idleHandle;
		_nowUpdateState = &Player::IdleUpdate;
		(this->*_nowUpdateState)();
		return;
	}

	// ���˓��͂���������
	if (Input::GetInstance().IsTrigger(INPUTSHOT))
	{
		// �e��ǉ�(����)������
		// ���ۂɔ��˂����邩�ǂ����͌������Ō��߂�
		_bulletController->AddBullet(_pos, _isReverseGraphX);
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
	_vel = dir * PlayerConstData::kMoveSpeed;

	// �������x�̍X�V
	_fallSpeed += PlayerConstData::kFallSpeed;

	// �n�ʂƐڂ��Ă��邩�̔���
	// �ڂ��Ă��邩�ǂ�����bool��Ԃ�
	bool isGround = CheckIsBottom();

	// ��Ɛڂ��Ă��邩�̔���
	// �܂��̊֐��Ƃقړ���
	// bool�Ԃ��Ă��邪�����o�ϐ��ɕۑ����Ă��邽�ߎg��Ȃ���
	CheckIsTop();

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;


	// �e�̍X�V����
	_bulletController->Update(_map);
}

bool Player::CheckIsBottom() const
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
			std::shared_ptr<MapChip> chip = _map->GetMapChipData(chipX, chipY);

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
		// �������x���[���ɂ��A�}���ȗ�����h��
		//_fallSpeed = 0.0f;

		// ��X�̎G�v�Z�ɋ����̂ŁAy���W�̏����_��������΂�
		//_pos.y = floorf(_pos.y);	// ���傤�ǒn�ʂɕt���ʒu��
	}

	return isHit;
}

bool Player::CheckIsTop()
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
			std::shared_ptr<MapChip> chip = _map->GetMapChipData(chipX, chipY);

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
			isHit = _map->CheckHitMapChip(checkPos, _colSize, chipX, chipY);
			if (isHit)
			{
				// �������x�𔽓]
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
	// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
	int animNo = _animFrameCount / PlayerConstData::kSingleAnimFrame;

	// �\���ʒu(���S)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.GetDrawOffset().y);

	// �؂�o���J�n�ʒu
	int cutX = animNo * PlayerConstData::kGraphWidth;
	int cutY = 0;

	// �؂�o����
	int width  = PlayerConstData::kGraphWidth;
	int height = PlayerConstData::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		PlayerConstData::kSizeMul, 0.0,
		_useHandle, true, _isReverseGraphX, false);

	// �e�̕`�揈��
	_bulletController->Draw(camera);

#ifdef DISP_COLLISION

	DispCol(camera);

	DrawFormatString(96, 96, 0xffffff, L"pos : %.02f/%.02f", _pos.x, _pos.y);
	DrawFormatString(96, 112, 0xffffff, L"vel : %.02f/%.02f", _vel.x, _vel.y);
	DrawFormatString(96, 128, 0xffffff, L"colSize : %d/%d", _colSize.width, _colSize.height);

#endif // DISP_COLLISION
}