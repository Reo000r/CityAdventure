#pragma once
#include "BoxCollider.h"
#include <memory>
#include "Game.h"

//class GameSceneCamera;
class Map;
class PlayerBulletController;

namespace PlayerConstData
{
	constexpr float kSizeMul = 2.0f;

	// �L�����N�^�[�O���t�B�b�N�̕��ƍ���
	constexpr int kGraphWidth = 48;
	constexpr int kGraphHeight = 48;

	// �����n�_���W
	constexpr int kStartPosX = Game::kScreenCenterWidth;
	constexpr int kStartPosY = 500;//Game::kScreenCenterHeight;
	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 2;
	// ����̕��ƍ���
	constexpr int kColWidth = 24 * kSizeMul;
	constexpr int kColHeight = 32 * kSizeMul;
	// �����蔻��̕␳�l
	constexpr int kColPosOffsetX = 0;
	constexpr int kColPosOffsetY = 0;//(kColHeight / 2) * -1;

	// �L�����N�^�[�̕��ƍ���
	constexpr int kWidth = kGraphWidth * kSizeMul;
	constexpr int kHeight = kGraphHeight * kSizeMul;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 5;

	// �A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 5;
	constexpr int kRunAnimNum  = 6;
	constexpr int kJumpAnimNum = 2;
	constexpr int kFallAnimNum = 1;

	// �A�j���[�V�������������̃t���[����
	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
	constexpr int kJumpAnimTotalFrame = kJumpAnimNum * kSingleAnimFrame;
	constexpr int kFallAnimTotalFrame = kFallAnimNum * kSingleAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 2.5f * kSizeMul;

	// �W�����v�͂Əd��
	constexpr float kJumpForce = -9.2f * kSizeMul;
	constexpr float kFallSpeed = 0.4f * kSizeMul;

}

class Player : public BoxCollider
{
private:
	int _useHandle;
	int _idleHandle;
	int _runHandle;
	int _jumpHandle;
	int _fallHandle;
	int _animFrameCount;
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

	std::shared_ptr<Map> _map;

	// �e�̔��ˊǗ����s��
	std::shared_ptr<PlayerBulletController> _bulletController;

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

	// �܂Ƃ߂���
	/// <summary>
	/// �������n�ʂɐڂ��Ă��邩�m�F����
	/// </summary>
	bool CheckIsBottom() const;
	/// <summary>
	/// �㑤���V��ɐڂ��Ă��邩�m�F����
	/// </summary>
	bool CheckIsTop();

public:
	Player();

	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Init(std::shared_ptr<Map> map, int idleHandle, int runHandle, int jumpHandle, int fallHandle);

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(GameSceneCamera camera);

	Vector2 GetPos() { return _pos; }
	//Vector2 GetCenterPos()
	//{
	//	Vector2 ans;
	//	ans.x = static_cast<int>(_pos.x);
	//	ans.y = static_cast<int>(_pos.y - kHeight * 0.5f);
	//	return ans;
	//}
};

