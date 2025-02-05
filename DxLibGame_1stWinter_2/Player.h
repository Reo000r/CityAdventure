#pragma once
#include "BoxCollider.h"
#include <memory>
#include "Game.h"

//class GameSceneCamera;
class GamePlayScene;
class GoalObject;
class Map;
class PlayerBulletController;

namespace PlayerData
{
	// �ϋv��
	constexpr int kHitPoint = 15;
	
	// ���G����
	constexpr int kUnHitTime = 60 * 2;

	// ���������Ƃ݂Ȃ���鍂��
	// ���ֈړ�
	//constexpr int kDeadHeight = 1024 + 192 + kColWidth * 0.5f;

	// �����n�_���W
	constexpr int kStartPosX = 360;
	constexpr int kStartPosY = 2240;



	constexpr float kSizeMul = 2.0f;

	// �L�����N�^�[�O���t�B�b�N�̕��ƍ���
	constexpr int kGraphWidth = 48;
	constexpr int kGraphHeight = 48;

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 2;
	// ����̕��ƍ���
	constexpr int kColWidth  = static_cast<int>(24 * kSizeMul);
	constexpr int kColHeight = static_cast<int>(32 * kSizeMul);
	// �����蔻��̕␳�l
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;//(kColHeight / 2) * -1;

	// �L�����N�^�[�̕��ƍ���
	constexpr int kWidth  = static_cast<int>(kGraphWidth * kSizeMul);
	constexpr int kHeight = static_cast<int>(kGraphHeight * kSizeMul);

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 5;
	constexpr int kDeathAnimFrame = 10;

	// �A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 5;
	constexpr int kRunAnimNum  = 6;
	constexpr int kJumpAnimNum = 2;
	constexpr int kFallAnimNum = 1;
	constexpr int kDamageAnimNum = 1;
	constexpr int kDeathAnimNum = 8;

	// �A�j���[�V�������������̃t���[����
	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
	constexpr int kJumpAnimTotalFrame = kJumpAnimNum * kSingleAnimFrame;
	constexpr int kFallAnimTotalFrame = kFallAnimNum * kSingleAnimFrame;
	constexpr int kDamageAnimTotalFrame = kDamageAnimNum * kSingleAnimFrame;
	constexpr int kDeathAnimTotalFrame = kDeathAnimNum * kDeathAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 2.5f * kSizeMul;

	// �W�����v�͂Əd��
	constexpr float kJumpForce = -9.2f * kSizeMul;
	constexpr float kFallSpeed = 0.4f * kSizeMul;



	// ���������Ƃ݂Ȃ���鍂��
	constexpr int kDeadHeight = static_cast<int>(2241 + 32*10 + kColWidth * 0.5f);
}

class Player : public BoxCollider
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _jumpHandle;
	int _fallHandle;
	int _deathHandle;
	int _animFrameCount;
	
	// �ϋv��
	int _hitPoint;
	// ���G����
	int _unHitTime;
	bool _isActive;
	bool _isDead;
	bool _isGoal;
	bool _isHitTop;
	// ���Ȃ�true�A�E�Ȃ�false
	bool _isReverseGraphX;
	float _fallSpeed;
	// Player��_pos�͒��S
	//Vector2 _pos;
	Vector2 _vel;
	// �`�掞�̕␳�l(�Œ�l)
	// �O���t�B�b�N�̂���𒼂��ׂ̂���
	Vector2 _drawPosOffset;

	// �Q�[���I�[�o�[��`���邽��
	std::weak_ptr<GamePlayScene> _gamePlayScene;

	std::weak_ptr<GoalObject> _goal;

	std::weak_ptr<Map> _map;

	// �e�̔��ˊǗ����s��
	// �V�[����shared�������߁Aweak�Ŏ���
	std::weak_ptr<PlayerBulletController> _bulletController;

	// Update��State�p�^�[��
	// _update���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void(Player::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// �����쎞�̍X�V����
	/// </summary>
	void IdleUpdate();
	/// <summary>
	/// ���E���͎��̍X�V����
	/// </summary>
	void RunUpdate();
	/// <summary>
	/// �󒆎��̍X�V����
	/// JumpUpdate��FallUpdate���܂Ƃ߂�
	/// </summary>
	void AirUpdate();
	/// <summary>
	/// ��e���̍X�V����
	/// ���͂͌������A�����Ă�������Ƃ͋t�Ƀm�b�N�o�b�N
	/// </summary>
	void DamageUpdate();
	/// <summary>
	/// ���S���̍X�V����
	/// ���͂͌����Ȃ�
	/// </summary>
	void DeathUpdate();
	/// <summary>
	/// ���������̍X�V����
	/// ���͂͌����Ȃ�
	/// </summary>
	void FallDeathUpdate();

	// �܂Ƃ߂���
	/// <summary>
	/// �������n�ʂɐڂ��Ă��邩�m�F����
	/// </summary>
	bool IsHitBottom() const;
	/// <summary>
	/// �㑤���V��ɐڂ��Ă��邩�m�F����
	/// </summary>
	bool IsHitTop();

	/// <summary>
	/// �S�[���Ɠ������Ă��邩�m�F
	/// </summary>
	bool IsHitGoal() const;
	/// <summary>
	/// �����ŏ�O�ɏo�������肷��
	/// �o�Ă����玀
	/// </summary>
	/// <returns>true���Ԃ��Ă��Ă����玀��ł���̂ő�Update���Ă�return</returns>
	bool IsFallOutOfPlayArea();

public:
	Player();

	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Init(/*std::weak_ptr<GamePlayScene> gamePlayScene, */std::weak_ptr<GoalObject> goal, std::weak_ptr<Map> map, std::weak_ptr<PlayerBulletController> playerBulletController, int idleHandle, int runHandle, int jumpHandle, int fallHandle, int deathHandle);

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

	/// <summary>
	/// �_���[�W��H��������̏���
	/// </summary>
	/// <param name="">���]�t���O�Ɠ������A���Ȃ�true�A�E�Ȃ�false</param>
	void OnDamage(int damage, bool isReverseX);

	Vector2 GetPos() const { return _pos; }

	bool GetActiveStats() const { return _isActive; }
	int GetMaxHitpoint() const { return PlayerData::kHitPoint; }
	int GetCurrentHitpoint() const { return _hitPoint; }

	bool IsDead() const { return _isDead; }
	bool IsGoal() const { return _isGoal; }
};

