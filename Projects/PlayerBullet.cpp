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
	// �񊈐�����ԂȂ�return
	if (!_isActive) return;

	_lifeTimeCount--;
	// �������Ԃ𒴂��Ă�����
	if (_lifeTimeCount <= 0 || 
		_pos.x <= 0.0f)
	{
		//�񊈐���
		DisActive();
		return;
	}

	_animFrameCount++;
	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= PlayerBulletData::kAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	// �ړ�����
	if (!_isReverseGraphX)
	{
		// �E
		_vel.x = PlayerBulletData::kMoveSpeed;
	}
	else
	{
		// ��
		_vel.x = -PlayerBulletData::kMoveSpeed;
	}
	_vel.y = 0.0f;

	// �����蔻��
	Vector2 returnVel = map.lock()->CheckHitAllMapChip(_pos, _vel, _colSize);

	// �ړ��x�N�g�����C�����ꂽ�ꍇ�͓������Ă���
	if (_vel.x != returnVel.x || _vel.y != returnVel.y)
	{
		// �������Ă����ꍇ�͔񊈐���
		DisActive();
		return;
	}

	// �ړ�
	_pos += _vel;
}

void PlayerBullet::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// �񊈐�����ԂȂ�return
	if (!_isActive) return;

	// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
	int animNo = _animFrameCount / PlayerBulletData::kSingleAnimFrame;

	// �\���ʒu(���S)
	int x = static_cast<int>(_pos.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + camera.lock()->GetDrawOffset().y);

	// �؂�o���J�n�ʒu
	int cutX = animNo * PlayerBulletData::kGraphWidth;
	int cutY = 0;

	// �؂�o����
	int width  = PlayerBulletData::kGraphWidth;
	int height = PlayerBulletData::kGraphHeight;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		PlayerBulletData::kSizeMul, 0.0,
		_graphHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	// �f�o�b�O�`��
	DispCol(camera);

#endif // _DEBUG
}

bool PlayerBullet::Active(Vector2 pos, bool isReverse)
{
	// �����������ς̏ꍇ��false
	if (_isActive) return false;

	_isActive = true;
	// ���Ȃ�true�A�E�Ȃ�false
	_isReverseGraphX = isReverse;

	// �������Ԃ̐ݒ�
	_lifeTimeCount = PlayerBulletData::kLifeTime;

	_pos = pos;
	// �����ʒu�̕␳
	Vector2 offset = PlayerBulletData::kActivePosOffset;
	// x���̔��]����
	if (_isReverseGraphX)
	{
		offset.x *= -1;
	}
	// �␳
	_pos += offset;

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	return _isActive;
}

void PlayerBullet::DisActive()
{
	// �e���Ă���͂������O�̂���
	if (!_isActive) return;

	_isActive = false;
}