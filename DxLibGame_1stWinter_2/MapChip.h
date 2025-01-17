#pragma once
#include "BoxCollider.h"

namespace MapGraphData
{
	// �g�p����摜�̃T�C�Y
	constexpr int kGraphSizeX = 32;
	constexpr int kGraphSizeY = 32;


	// �}�b�v�̏c���ɕ~���}�b�v�`�b�v�̐�
	constexpr int kMapSizeX = 100;
	constexpr int kMapSizeY = 20;

	// ���摜�Ɋ܂܂�Ă���`�b�v�̐�
	constexpr int kNumOfChipInDataX = 9;
	constexpr int kNumOfChipInDataY = 6;
	constexpr int kNumOfAllChipInData = kNumOfChipInDataX * kNumOfChipInDataY;
}

namespace MapColData
{
	// ����ƕ\���̔{��
	// MapChipColData���̕ϐ��S�ĂɊ|�����Ă��邽�߁A�V���Ɋ|����K�v�͂Ȃ�
	// ���ʂȎ���Ȃ�����g��Ȃ�
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

	// _pos ��BoxCollider�Ɋ܂܂�Ă���

	// �Q�Ƃ���}�b�v�`�b�v�ԍ�
	int _mapChipNum;

public:

	// �R���X�g���N�^(��)
	/// <summary>
	/// �R���X�g���N�^(��)
	/// </summary>
	/// <param name="pos"       >�Q�[����̈ʒu</param>
	/// <param name="posInGraph">���摜����g�p����摜�̈ʒu(����)</param>
	/// <param name="graphSize" >���摜����g�p����摜�̃T�C�Y</param>
	/// <param name="colSize"   >���ۂ̓����蔻��̃T�C�Y</param>
	/// <param name="mapChipNum">�Q�Ƃ���}�b�v�`�b�v�ԍ�</param>
	/// <param name="sizeMul"   >�\���{��</param>
	//MapChip(Vector2 pos, Vector2 posInGraph, Game::Size graphSize, Game::Size colSize, int mapChipNum, float sizeMul);
	
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
	void Draw(GameSceneCamera camera, int mapGraphHandle);

	/// <summary>
	/// ���݈ʒu��Ԃ�
	/// </summary>
	/// <returns></returns>
	Vector2 GetPos()          { return _pos; }

	/// <summary>
	/// �Q�Ƃ��Ă���}�b�v�`�b�v�ԍ���Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetMapChipNum()       { return _mapChipNum; }

	/// <summary>
	/// ���摜�̍����琔�����ʒu
	/// </summary>
	/// <returns></returns>
	int GetNumOnDataX() { return _mapChipNum % MapGraphData::kNumOfChipInDataX; }
	
	/// <summary>
	/// ���摜�̏ォ�琔�����ʒu
	/// </summary>
	/// <returns></returns>
	int GetNumOnDataY() { return _mapChipNum / MapGraphData::kNumOfChipInDataX; }

	/// <summary>
	/// �g�p����摜�̃T�C�Y
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipGraphSize()
	{
		Game::Size ans;
		ans.width  = MapGraphData::kGraphSizeX;
		ans.height = MapGraphData::kGraphSizeY;
		return ans;
	}

	/// <summary>
	/// �}�b�v�`�b�v�̔���T�C�Y
	/// </summary>
	/// <returns></returns>
	Game::Size GetChipColSize()
	{
		Game::Size ans;
		ans.width  = MapColData::kColSizeX;
		ans.height = MapColData::kColSizeY;
		return ans;
	}

	/// <summary>
	/// ���摜���ł̈ʒu��Ԃ�
	/// </summary>
	/// <returns></returns>
	Vector2 GetPosInGraph()
	{
		Vector2 ans;
		ans.x = GetNumOnDataX() * MapGraphData::kGraphSizeX;
		ans.y = GetNumOnDataY() * MapGraphData::kGraphSizeY;
		return ans;
	}
};

