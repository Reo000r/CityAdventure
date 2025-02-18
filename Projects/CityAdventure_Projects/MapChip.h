#pragma once
#include "BoxCollider.h"

#include <memory>

namespace MapGraphData
{
	// �g�p����摜�̃T�C�Y
	constexpr int kGraphSizeX = 32;
	constexpr int kGraphSizeY = 32;


	// �}�b�v�̏c���ɕ~���}�b�v�`�b�v�̐�
	constexpr int kMapSizeX = 100;
	constexpr int kMapSizeY = 40;

	// ���摜�Ɋ܂܂�Ă���`�b�v�̐�
	constexpr int kNumOfChipInDataX = 9;
	constexpr int kNumOfChipInDataY = 6;
	constexpr int kNumOfAllChipInData = kNumOfChipInDataX * kNumOfChipInDataY;
}

namespace MapColData
{
	// ����ƕ\���̔{��
	// MapChipColData���̕ϐ��S�ĂɊ|�����Ă��邽�߁A�V���Ɋ|����K�v�͂Ȃ�
	// ���ʂȎ���Ȃ����葼�ł͎g��Ȃ�
	constexpr int kSizeMul = 2;

	// �}�b�v�`�b�v�̔���T�C�Y
	constexpr int kColSizeX = MapGraphData::kGraphSizeX * kSizeMul;
	constexpr int kColSizeY = MapGraphData::kGraphSizeY * kSizeMul;
}

/// <summary>
/// �}�b�v�`�b�v��ЂƂ̏��
/// �܂Ƃ߂�Map���z��Ƃ��Ď����Ă���
/// </summary>
class MapChip : public BoxCollider
{
private:

	// �Q�Ƃ���}�b�v�`�b�v�ԍ�
	int _mapChipNum;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos"       >�Q�[����̈ʒu</param>
	/// <param name="mapChipNum">�Q�Ƃ���}�b�v�`�b�v�ԍ�</param>
	MapChip(Vector2 pos, int mapChipNum);

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera, int mapGraphHandle) const;

	/// <summary>
	/// ���݈ʒu��Ԃ�
	/// </summary>
	Vector2 GetPos() const    { return _pos; }

	/// <summary>
	/// �Q�Ƃ��Ă���}�b�v�`�b�v�ԍ���Ԃ�
	/// </summary>
	int GetMapChipNum() const { return _mapChipNum; }

	/// <summary>
	/// ���摜�̍����琔�����ʒu
	/// </summary>
	int GetNumOnDataX() const { return _mapChipNum % MapGraphData::kNumOfChipInDataX; }
	
	/// <summary>
	/// ���摜�̏ォ�琔�����ʒu
	/// </summary>
	int GetNumOnDataY() const { return _mapChipNum / MapGraphData::kNumOfChipInDataX; }

	/// <summary>
	/// �g�p����摜�̃T�C�Y
	/// </summary>
	Game::Size GetChipGraphSize() const
	{
		Game::Size ans;
		ans.width  = MapGraphData::kGraphSizeX;
		ans.height = MapGraphData::kGraphSizeY;
		return ans;
	}

	/// <summary>
	/// �}�b�v�`�b�v�̔���T�C�Y
	/// </summary>
	Game::Size GetChipColSize() const
	{
		Game::Size ans;
		ans.width  = MapColData::kColSizeX;
		ans.height = MapColData::kColSizeY;
		return ans;
	}

	/// <summary>
	/// ���摜���ł̈ʒu��Ԃ�
	/// </summary>
	Vector2 GetPosInGraph() const
	{
		Vector2 ans;
		ans.x = static_cast<float>(GetNumOnDataX() * MapGraphData::kGraphSizeX);
		ans.y = static_cast<float>(GetNumOnDataY() * MapGraphData::kGraphSizeY);
		return ans;
	}
};

