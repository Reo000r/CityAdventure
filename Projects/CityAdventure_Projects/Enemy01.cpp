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
	// �v���C���[�̈��͈͂ɓ���܂őҋ@���
	// �����Ă����瑖��n�߂�
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

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= Enemy01Data::kIdleAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	_blinkFrameCount--;
	if (_blinkFrameCount <= 0) _blinkFrameCount = 0;

	// �e�Ɠ����������ǂ���
	Game::Rect rect = GetRect();
	// �擾�p
	std::weak_ptr<PlayerBullet> bullet;
	if (_playerBulletController.lock()->IsHitBullet(rect, bullet))
	{
		// �e��������荶���ɂ����true
		bool isReverseX = bullet.lock()->GetPos().x < _pos.x;
		int damage = bullet.lock()->GetDamage();
		// ���������玩���Ƀ_���[�W
		OnDamage(damage, isReverseX);
	}

	// �v���C���[�Ɠ����蔻����s��
	IsPlayerHit();
}

void Enemy01::RunUpdate()
{
	// �v���C���[�̈��͈͂ɓ����Ă��Ȃ�������ҋ@��Ԃ�
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

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= Enemy01Data::kRunAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	_blinkFrameCount--;
	if (_blinkFrameCount <= 0) _blinkFrameCount = 0;

	// �e�Ɠ����������ǂ���
	Game::Rect rect = GetRect();
	// �擾�p
	std::weak_ptr<PlayerBullet> bullet;
	if (_playerBulletController.lock()->IsHitBullet(rect, bullet))
	{
		// �e��������荶���ɂ����true
		bool isReverseX = bullet.lock()->GetPos().x < _pos.x;
		int damage = bullet.lock()->GetDamage();
		// ���������玩���Ƀ_���[�W
		OnDamage(damage, isReverseX);
	}

	// �����ɒn�ʂ����邩�m�F
	bool isGround = IsHitBottom();

	// �����ɒn�ʂ�����������
	if (!isGround)
	{
		// Fall�̐ݒ�����Ă���Air�֐������s����return
		_fallSpeed = 0.0f;
		_animFrameCount = 0;
		_useHandle = _fallHandle;
		_nowUpdateState = &Enemy01::AirUpdate;
		(this->*_nowUpdateState)();
		return;
	}


	Vector2 dir = { 0.0f, 0.0f };

	// �����Ă����������ړ����������߂�
	if (!_isReverseGraphX)
	{
		// �E
		dir.x += 1.0f;
	}
	else
	{
		// ��
		dir.x -= 1.0f;
	}

	// ���K��
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// ���E�ړ��ʂ��o��
	_vel = dir * Enemy01Data::kMoveSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;

	// �v���C���[�Ɠ����蔻����s��
	IsPlayerHit();

	// ���E�ɕǂ����邩�𔻒�
	if (IsRightAndLeft())
	{
		// �������ꍇ�͔��]
		_isReverseGraphX = !_isReverseGraphX;
	}


}

void Enemy01::AirUpdate()
{
	// �v���C���[�̈��͈͂ɓ���܂�Update���~�߂�
	Vector2 playerPos = _player.lock()->GetPos();
	if ((playerPos.x + Game::kScreenCenterWidth + Enemy01Data::kActivationRange <= _pos.x) ||
		(playerPos.x - Game::kScreenCenterWidth - Enemy01Data::kActivationRange >= _pos.x)) return;

	_animFrameCount++;
	
	// ���~��
	// �A�j���[�V������1���z��
	_animFrameCount = 0;

	_blinkFrameCount--;
	if (_blinkFrameCount <= 0) _blinkFrameCount = 0;

	// �e�Ɠ����������ǂ���
	Game::Rect rect = GetRect();
	// �擾�p
	std::weak_ptr<PlayerBullet> bullet;
	if (_playerBulletController.lock()->IsHitBullet(rect, bullet))
	{
		// �e��������荶���ɂ����true
		bool isReverseX = bullet.lock()->GetPos().x < _pos.x;
		int damage = bullet.lock()->GetDamage();
		// ���������玩���Ƀ_���[�W
		OnDamage(damage, isReverseX);
	}

	// �����ɒn�ʂ����邩���ɉ������Ă���Ȃ�
	if (IsHitBottom() && _fallSpeed >= 0.0f)
	{
		// Run�֐������s����return
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Enemy01::RunUpdate;
		(this->*_nowUpdateState)();
		return;
	}



	Vector2 dir = { 0.0f, 0.0f };

	// �����Ă����������ړ����������߂�
	if (!_isReverseGraphX)
	{
		// �E
		dir.x += 1.0f;
	}
	else
	{
		// ��
		dir.x -= 1.0f;
	}

	// ���K��
	if (dir.SqrMagnitude(dir) > 0)
	{
		dir = dir.Normalized(dir);
	}

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// ���E�ړ��ʂ��o��
	_vel = dir * Enemy01Data::kMoveSpeed;

	// �������x�̍X�V
	_fallSpeed += Enemy01Data::kFallSpeed;

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;

	// �v���C���[�Ɠ����蔻����s��
	IsPlayerHit();

	// ���E�ɕǂ����邩�𔻒�
	if (IsRightAndLeft())
	{
		// �������ꍇ�͔��]
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

	return isHit;
}

bool Enemy01::IsRightAndLeft()
{
	// �E

	// 1�h�b�g�E�ɂ��炵�A�������Ă���ΕǂɐG��Ă���
	Vector2 checkPos = _pos;
	checkPos.x += 1.0f;

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
				// �������Ă���Ώ������΂�
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

	// ��

	// 1�h�b�g���ɂ��炵�A�������Ă���ΕǂɐG��Ă���
	checkPos = _pos;
	checkPos.x -= 1.0f;

	// �S�}�b�v�`�b�v�������蔻����s��
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
				// ����
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
	// �񊈐�����ԂȂ��΂�
	if (!_player.lock()->GetActiveStats()) return false;

	// ����(�G)�̋�`�����擾
	Game::Rect rect = GetRect();
	// �v���C���[�̓����蔻����擾
	Game::Rect playerRect = _player.lock()->GetRect();

	// �e�Ɠ����蔻��
	bool isHit = !(
		(rect.top >= playerRect.bottom || rect.bottom <= playerRect.top) ||
		(rect.left >= playerRect.right || rect.right <= playerRect.left));
	// �������Ă�����
	if (isHit)
	{
		Vector2 playerPos = _player.lock()->GetPos();
		// �������v���C���[��荶���ɂ����true
		bool isReverseX = _pos.x < playerPos.x;

		// �v���C���[�ɓ����������Ƃ�`����
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

	// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
	int animNo = _animFrameCount / Enemy01Data::kSingleAnimFrame;

	// �\���ʒu(���S)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// �؂�o���J�n�ʒu
	int cutX = animNo * Enemy01Data::kGraphWidth;
	int cutY = 0;

	// �؂�o����
	int width  = Enemy01Data::kGraphWidth;
	int height = Enemy01Data::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		_sizeMul, _angle,
		_useHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	// �f�o�b�O�`��
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

	// �ϋv�͂�0�ȉ��ɂȂ�����񊈐���
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
