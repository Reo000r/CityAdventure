#include "EnemyBullet.h"
#include "Map.h"
#include "Player.h"


bool EnemyBullet::IsHitPlayer()
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
	// Controller���Œe���Ă���͂������O�̂���
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
	if (_animFrameCount >= EnemyBulletData::kAnimTotalFrame)
	{
		_animFrameCount = 0;
	}

	_vel = _dir * EnemyBulletData::kMoveSpeed;

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
	
	if (IsHitPlayer())
	{
		DisActive();
	}
}

void EnemyBullet::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// Controller���Œe���Ă���͂������O�̂���
	if (!_isActive) return;

	// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
	int animNo = _animFrameCount / EnemyBulletData::kSingleAnimFrame;

	// �\���ʒu(���S)
	int x = static_cast<int>(_pos.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + camera.lock()->GetDrawOffset().y);

	// �؂�o���J�n�ʒu
	int cutX = animNo * EnemyBulletData::kGraphWidth;
	int cutY = 0;

	// �؂�o����
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
	// �����������ς̏ꍇ��false
	if (_isActive) return false;

	_isActive = true;
	// ���Ȃ�true�A�E�Ȃ�false
	_isReverseGraphX = isReverse;

	// �������Ԃ̐ݒ�
	_lifeTimeCount = EnemyBulletData::kLifeTime;

	_pos = pos;
	
	Vector2 playerPos = _player.lock()->GetPos();

	_dir = playerPos - _pos;
	// Vector2���C���悤...
	_dir = _dir.Normalized(_dir);

	_vel.x = 0.0f;
	_vel.y = 0.0f;

	return _isActive;
}

void EnemyBullet::DisActive()
{
	// �e���Ă���͂������O�̂���
	if (!_isActive) return;

	_isActive = false;
}