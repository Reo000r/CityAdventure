#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;
class GameSceneCamera;

namespace
{
	constexpr float kSizeMul = 2.0f;

	// �g�p����摜�̃T�C�Y
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;

	// �����蔻��̕��ƍ���
	constexpr int kColWidth = 16 * kSizeMul;
	constexpr int kColHeight = 6 * kSizeMul;

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 6;
	// �`�掞�̕␳�l(�Œ�l)
	// �O���t�B�b�N�̂���𒼂��ׂ̂���
	Vector2 kDrawPosOffset;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 4;

	// �A�j���[�V�����̃R�}��
	constexpr int kAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 3.5f * kSizeMul;
}

/// <summary>
/// �G�̊��N���X
/// </summary>
class Enemy : public BoxCollider
{
private:
	int _graphHandle;
	int _animFrameCount;
	// ���Ȃ�true�A�E�Ȃ�false
	bool _isReverseGraphX;

	// true�Ȃ犈����
	// Update��Draw��DisActive���󂯕t����
	// false�Ȃ�񊈐���
	// Active���󂯕t����
	bool _isActive;

	// _pos�͒��S
	Vector2 _vel;

public:

	Enemy(int graphHandle);
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update(std::shared_ptr<Map> map);

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(GameSceneCamera camera);

	/// <summary>
	/// ������
	/// </summary>
	bool Active(Vector2 pos, bool isReverse);

	/// <summary>
	/// �񊈐���
	/// </summary>
	void DisActive();

	/// <summary>
	/// ��������Ԃ��擾
	/// </summary>
	bool GetActiveStats() { return _isActive; }


};
