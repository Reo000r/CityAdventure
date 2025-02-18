#pragma once

//#define LOAD_DEBUG_STAGE
#define RELOAD_GAME_STAGE

namespace Game
{
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	constexpr float kScreenCenterWidth = kScreenWidth * 0.5f;
	constexpr float kScreenCenterHeight = kScreenHeight * 0.5f;

	constexpr int kFadeInterval = 30;

	constexpr int kSoundVolume = 128;

	// �����`�悵�Ȃ��}�b�v�ԍ�
	constexpr int kEmptyChipNum = 51;

	constexpr int kNotColChipNum1 = 51;  // �`��Ȃ�
	constexpr int kNotColChipNum2 = 10;  // �w�i
	constexpr int kNotColChipNum3 = 39;  // ���̔�
	constexpr int kNotColChipNum4 = 12;  // �w�i�Ƃ̌q���̕�
	constexpr int kNotColChipNum5 = 14;  // �w�i�Ƃ̌q���̕�



	// ��`�̏c������ۑ�����\����
	struct Size
	{
		int width;
		int height;
	};

	// �ʒu������������`�̏���ۑ�����\����
	// �����o�ϐ��Ƃ��Ď������鎖�͑z�肵�Ă��Ȃ�
	// GetRect()������肻���Ŏ擾���g��
	struct Rect
	{
		// ��
		int top;
		// ��
		int bottom;
		// �E
		int right;
		// ��
		int left;
	};

	/// <summary>
	/// ������c���}�b�v�`�b�v�@�������@�ꍇ��false
	/// �@�@�@�@�@�@�@�@�@�@�@�łȂ������ꍇ��true
	/// ����3��ނƔ���
	/// </summary>
	bool CheckEmptyMapChip(int chipNum);
	
}

