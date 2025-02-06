#pragma once
#include "Enemy.h"
#include "Player.h"

class EnemyBulletController;

namespace BossEnemyData
{
	// �ϋv��
	constexpr int kHitPoint = 40;

	// �U����
	constexpr int kAddDamageAmount = 1;



	constexpr float kSizeMul = 8.0f;

	// �L�����N�^�[�O���t�B�b�N�̕��ƍ���
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = -18;
	// ����̕��ƍ���
	constexpr int kColWidth = 16 * kSizeMul;
	constexpr int kColHeight = 16 * kSizeMul;
	// �����蔻��̕␳�l
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;//(kColHeight / 2) * -1;

	// �L�����N�^�[�̕��ƍ���
	constexpr int kWidth = kGraphWidth * kSizeMul;
	constexpr int kHeight = kGraphHeight * kSizeMul;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 5;

	// �A�j���[�V�����̃R�}��
	constexpr int kFlyAnimNum = 6;
	constexpr int kDeathAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kFlyAnimTotalFrame = kFlyAnimNum * kSingleAnimFrame;
	constexpr int kDeathAnimTotalFrame = kDeathAnimNum * kSingleAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 2.0f * kSizeMul;

	// �d��
	constexpr float kFallSpeed = 0.4f * kSizeMul;

	// �v���C���[�����̋������ɂ����瓮��
	constexpr int kActivationRange = 1000;



	// �������x���
	constexpr float kMaxFallSpeed = 5.0f;


	// �e�����Ԋu
	constexpr int kBulletCooltime = 90;

	// ��]���x
	// �傫���قǒx��
	constexpr float kRotateSpeed = 20.0f * 1.5f;

	// ���a�̑傫���{��
	constexpr float kRadMul = 0.8f * kSizeMul;

	// �����ʒu�␳
	constexpr int kStartPosOffset = kRadMul * kSizeMul * 2.5f;
}

class BossEnemy : public Enemy
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _fallHandle;
	int _animFrameCount;
	bool _isHitTop;

	// �؋󎞊�
	int _flyFrameCount;

	int _bulletCooltimeFrame;

	bool _isDead;
	int  _deadFrameCount;
	float _sizeMul;
	float _angle;
	float _alpha;

	float _fallSpeed;
	//Vector2 _pos;
	//Vector2 _vel;
	// �`�掞�̕␳�l(�Œ�l)
	// �O���t�B�b�N�̂���𒼂��ׂ̂���
	Vector2 _drawPosOffset;

	std::weak_ptr<Player> _player;
	std::weak_ptr<PlayerBulletController> _playerBulletController;
	std::weak_ptr<EnemyBulletController> _enemyBulletController;

	// Update��State�p�^�[��
	// _update���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void(BossEnemy::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// �󒆎��̍X�V����
	/// </summary>
	void FlyUpdate();
	/// <summary>
	/// ���S���̍X�V����
	/// </summary>
	void DeathUpdate();

	/// <summary>
	/// �v���C���[�Ɠ����蔻����s��
	/// </summary>
	/// <returns></returns>
	bool IsPlayerHit();

	/// <summary>
	/// �v���C���[�Ƃ̋����𑪂�
	/// </summary>
	/// <returns>������Ȃ�true</returns>
	bool CheckPlayerDistance()
	{
		Vector2 playerPos = _player.lock()->GetPos();
		float x = _pos.x * 2 - playerPos.x * 2;
		float y = _pos.y * 2 - playerPos.y * 2;
		if (x < 0.0f) x *= -1;
		if (y < 0.0f) y *= -1;
		float blockNumX = 25.0f;
		float blockNumY = 8.0f;
		int blockSize = 32;
		float blockMul = 2.0f;
		//return (_pos.Distance(_pos, playerPos) > Enemy01Data::kActivationRange);
		return (x <= blockNumX * blockSize * blockMul &&
			(y <= blockNumY * blockSize * blockMul));
	}

public:
	BossEnemy(Vector2 pos);

	~BossEnemy();

	/// <summary>
	/// ������
	/// </summary>
	void Init(std::weak_ptr<Map> map,
		std::weak_ptr<Player> player,
		std::weak_ptr<PlayerBulletController> playerBulletController,
		int idleHandle,
		int runHandle,
		int fallHandle) override;

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera) override;

	/// <summary>
	/// �`��S��
	/// </summary>
	bool Active(Vector2 pos, bool isReverse) override;

	/// <summary>
	/// �`��S��
	/// </summary>
	void DisActive() override;

	/// <summary>
	/// �_���[�W��H��������̏���
	/// </summary>
	void OnDamage(int damage, bool isReverseX) override;


	void SetEnemyBulletController(std::weak_ptr<EnemyBulletController> enemyBulletController) 
	{
		_enemyBulletController = enemyBulletController;
	}

	Vector2 GetPos() const { return _pos; }

	bool IsDead() { return _isDead; }
};

