#pragma once
#include "Enemy.h"
#include "Player.h"

namespace Enemy01Data
{
	// �ϋv��
	constexpr int kHitPoint = 2;

	// �U����
	constexpr int kAddDamageAmount = 1;
	

	
	constexpr float kSizeMul = 2.0f;

	// �L�����N�^�[�O���t�B�b�N�̕��ƍ���
	constexpr int kGraphWidth  = 48;
	constexpr int kGraphHeight = 48;

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = -24;
	// ����̕��ƍ���
	constexpr int kColWidth  = 32 * kSizeMul;
	constexpr int kColHeight = 24 * kSizeMul;
	// �����蔻��̕␳�l
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;

	// �L�����N�^�[�̕��ƍ���
	constexpr int kWidth  = kGraphWidth  * kSizeMul;
	constexpr int kHeight = kGraphHeight * kSizeMul;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 5;

	// �A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 6;
	constexpr int kFallAnimNum = 1;

	// �A�j���[�V�������������̃t���[����
	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
	constexpr int kFallAnimTotalFrame = kFallAnimNum * kSingleAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 2.0f * kSizeMul;

	// �d��
	constexpr float kFallSpeed = 0.4f * kSizeMul;




	// �v���C���[�����̋������ɂ����瓮��
	constexpr int kActivationRange = 500;
}

/// <summary>
/// �n��G
/// �����A�ǂɓ��������甽�]����
/// </summary>
class Enemy01 : public Enemy
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _fallHandle;
	int _animFrameCount;
	bool _isHitTop;

	bool _isDead;
	int _deadFrameCount;
	float _sizeMul;
	float _angle;

	float _fallSpeed;

	// �`�掞�̕␳�l(�Œ�l)
	// �O���t�B�b�N�̂���𒼂��ׂ̂���
	Vector2 _drawPosOffset;

	std::weak_ptr<Player> _player;
	std::weak_ptr<PlayerBulletController> _playerBulletController;

	// Update��State�p�^�[��
	// _update���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void(Enemy01::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// �v���C���[�������ɂ��鎞�̍X�V����
	/// </summary>
	void IdleUpdate();
	/// <summary>
	/// �ʏ펞�̍X�V����
	/// </summary>
	void RunUpdate();
	/// <summary>
	/// �󒆎��̍X�V����
	/// </summary>
	void AirUpdate();
	/// <summary>
	/// ���S���̍X�V����
	/// </summary>
	void DeathUpdate();

	/// <summary>
	/// �������n�ʂɐڂ��Ă��邩�m�F����
	/// </summary>
	bool IsHitBottom() const;
	/// <summary>
	/// �E/�������ǂɐڂ��Ă��邩�m�F����
	/// </summary>
	bool IsRightAndLeft();
	
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
		float blockNumX = 27.0f;
		float blockNumY = 6.6f;
		int blockSize = 32;
		float blockMul = 2.0f;
		
		return (x <= blockNumX * blockSize * blockMul &&
			(y <= blockNumY * blockSize * blockMul));
	}

public:
	Enemy01(Vector2 pos);

	~Enemy01();

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


	Vector2 GetPos() const { return _pos; }
};

