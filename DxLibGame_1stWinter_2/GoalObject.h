#pragma once
#include "BoxCollider.h"

namespace GoalData
{
	
#ifdef LOAD_DEBUG_STAGE
	// �����n�_���W
	constexpr int kStartPosX = 72 * 32 * 2.0f;
	constexpr int kStartPosY = 3 * 32 * 2.0f + 4 * 2.0f;
#else
	constexpr int kStartPosX = 65 * 32 * 2.0f;
	constexpr int kStartPosY = 33 * 32 * 2.0f + 4 * 2.0f;
#endif // LOAD_DEBUG_STAGE

	// �g�p����摜�̕��ƍ���
	constexpr int kGraphWidth  = 48;
	constexpr int kGraphHeight = 48;

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = -24;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 9;

	// �A�j���[�V�����̃R�}��
	constexpr int kAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;;
}

namespace GoalColData
{
	// ����ƕ\���̔{��
	// MapColData���̕ϐ��S�ĂɊ|�����Ă��邽�߁A�V���Ɋ|����K�v�͂Ȃ�
	// ���ʂȎ���Ȃ�����g��Ȃ�
	constexpr int kSizeMul = 2;

	// ����T�C�Y
	constexpr int kColWidth  = 40  * kSizeMul;
	constexpr int kColHeight = 24 * kSizeMul;
}

/// <summary>
/// �S�[����������I�u�W�F�N�g
/// Map�ƍ��킹��������
/// </summary>
class GoalObject : public BoxCollider
{
	// _pos ��BoxCollider�Ɋ܂܂�Ă���

	int _graphHandle;
	int _animFrameCount;
	bool _isReverseGraphX;

	bool _isActive;
	float _alpha;

	Vector2 _drawPosOffset;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GoalObject();

	~GoalObject();

	/// <summary>
	/// ������
	/// </summary>
	void Init(int graphHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera) const;

	/// <summary>
	/// �g�p����摜�̃T�C�Y
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipGraphSize() const
	{
		Game::Size ans;
		ans.width = GoalData::kGraphWidth;
		ans.height = GoalData::kGraphHeight;
		return ans;
	}

	/// <summary>
	/// �}�b�v�`�b�v�̔���T�C�Y
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipColSize() const
	{
		Game::Size ans;
		ans.width = GoalColData::kColWidth;
		ans.height = GoalColData::kColHeight;
		return ans;
	}

	bool Active();

	bool IsActive() { return _isActive; }
};

