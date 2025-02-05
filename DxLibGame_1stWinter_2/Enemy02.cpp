#include "Enemy02.h"
#include "Map.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerBulletController.h"

#include "Input.h"

#include <cassert>

void Enemy02::FlyUpdate()
{
	// �v���C���[�̈��͈͂ɓ���܂�Update���~�߂�
	//Vector2 playerPos = _player.lock()->GetPos();
	//if (_pos.Distance(_pos, playerPos) >= Enemy02Data::kActivationRange) return;
	if (!CheckPlayerDistance()) return;

	_animFrameCount++;
	_flyFrameCount++;

	// �A�j���[�V�����̍��v�t���[�����𒴂�����Ō�Ŏ~�߂�
	if (_animFrameCount >= Enemy02Data::kFlyAnimTotalFrame)
	{
		_animFrameCount = 0;//Enemy02Data::kFlyAnimTotalFrame;
	}

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


	// �ړ�
	_vel = {};
	float s = _flyFrameCount / 15.0f;
	_vel.y = cos(s);
	_vel.y *= 4.0f;


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

void Enemy02::DeathUpdate()
{
	_deadFrameCount++;

	float maxDeadFrame = Enemy02Data::kDeathAnimTotalFrame * 2;
	
	if (_deadFrameCount > maxDeadFrame / 2.0f)
	{
		_alpha = (maxDeadFrame - _deadFrameCount / maxDeadFrame);
		_sizeMul -= 1 * Enemy02Data::kSizeMul / maxDeadFrame;
	}

	if (_deadFrameCount > maxDeadFrame)
	{
		DisActive();
		return;
	}


	_vel.x = 0.0f;
	_vel.y = 0.0f;

	// �������x�̍X�V
	_fallSpeed += Enemy02Data::kFallSpeed;

	if (_fallSpeed > Enemy02Data::kMaxFallSpeed)
	{
		_fallSpeed = Enemy02Data::kMaxFallSpeed;
	}

	// �ړ��ʂɗ������x��������
	_vel.y += _fallSpeed;

	// �����蔻����s���A�ǂɂ߂荞�܂Ȃ��悤�Ɉړ��x�N�g�����C��
	_vel = _map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ�
	_pos += _vel;
}

bool Enemy02::IsHitBottom() const
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

bool Enemy02::IsRightAndLeft()
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
				// �E��
				//_isReverseGraphX = false;
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

bool Enemy02::IsPlayerHit()
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
		_player.lock()->OnDamage(Enemy02Data::kAddDamageAmount, isReverseX);
		return true;
	}

	return false;
}

Enemy02::Enemy02(Vector2 pos) :
	Enemy(pos, { Enemy02Data::kColPosOffsetX, Enemy02Data::kColPosOffsetY }, { Enemy02Data::kColWidth,Enemy02Data::kColHeight }, Enemy02Data::kHitPoint),
	_useHandle(0),
	_idleHandle(0),
	_runHandle(0),
	_fallHandle(0),
	_animFrameCount(0),
	_isHitTop(false),
	_flyFrameCount(0),
	_isDead(false),
	_deadFrameCount(0),
	_sizeMul(Enemy02Data::kSizeMul),
	_alpha(0.0f),
	_fallSpeed(0.0f),
	_drawPosOffset({ Enemy02Data::kDrawPosOffsetX, Enemy02Data::kDrawPosOffsetY }),
	_nowUpdateState(&Enemy02::FlyUpdate)
{
}

Enemy02::~Enemy02()
{
	DeleteGraph(_idleHandle);
	DeleteGraph(_runHandle);
	DeleteGraph(_fallHandle);
}

void Enemy02::Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int fallHandle)
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

void Enemy02::Update()
{
	(this->*_nowUpdateState)();
}

void Enemy02::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
	int animNo = _animFrameCount / Enemy02Data::kSingleAnimFrame;

	// �\���ʒu(���S)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// �؂�o���J�n�ʒu
	int cutX = animNo * Enemy02Data::kGraphWidth;
	int cutY = 0;

	// �؂�o����
	int width = Enemy02Data::kGraphWidth;
	int height = Enemy02Data::kGraphHeight;

	if (_alpha != 0.0f)
	{
		float a = _deadFrameCount / (Enemy02Data::kDeathAnimTotalFrame * 2.0f);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
		
	}
	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		_sizeMul, 0.0,
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

bool Enemy02::Active(Vector2 pos, bool isReverse)
{
	if (_isActive) return false;

	_isActive = true;
	_pos = pos;
	_isReverseGraphX = isReverse;

	return true;
}

void Enemy02::DisActive()
{
	if (!_isActive) return;

	_isActive = false;
}

void Enemy02::OnDamage(int damage, bool isReverseX)
{
	_hitPoint -= damage;

	// �ϋv�͂�0�ȉ��ɂȂ�����񊈐���
	if (_hitPoint <= 0)
	{
		_vel.x = 0.0f;
		_animFrameCount = 0;
		_useHandle = _runHandle;
		_nowUpdateState = &Enemy02::DeathUpdate;
		_isDead = true;

		float s = _flyFrameCount / 15.0f;
		float fallSpeed = sin(s);
		fallSpeed *= 4.0f;
		_fallSpeed = fallSpeed;

		return;
	}
}
