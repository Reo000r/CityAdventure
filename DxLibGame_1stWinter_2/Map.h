#pragma once
#include "Game.h"
#include "MapChip.h"
#include <memory>

// MapChip.h�Ɉڂ���
/// <summary>
/// �}�b�v�̌��摜�֘A
/// </summary>
//namespace MapData
//{
//	// �}�b�v�`�b�v��ЂƂ֘A
//
//
//
//	constexpr float kSizeMul = 2.0f;
//
//	// �}�b�v�̉摜�T�C�Y(�����`)
//	//constexpr int kMapGraphSize = 512;
//
//	// �}�b�v�`�b�v�������̉摜�T�C�Y
//	constexpr int kMapChipGraphSize = 32;
//	//constexpr int kMapChipGraphWidth = 32;
//	//constexpr int kMapChipGraphHeight = 32;
//
//	// �}�b�v�̌��摜�̏c���̃`�b�v�̐�
//	constexpr int kMapGraphInChipNumX = 9;
//	constexpr int kMapGraphInChipNumY = 6;
//
//	// �}�b�v�`�b�v�������̕\���T�C�Y
//	constexpr int kChipDrawSize = kMapChipGraphSize * kSizeMul;
//	//constexpr int kChipWidth = kMapChipGraphWidth * kSizeMul;
//	//constexpr int kChipHeight = kMapChipGraphHeight * kSizeMul;
//
//	// ���f�[�^�Ɋ܂܂�Ă���}�b�v�`�b�v�̐�
//	constexpr int kChipDataNumX = 9;
//	constexpr int kChipDataNumY = 6;
//	constexpr int kChipDataNum = kChipDataNumX * kChipDataNumY;
//
//	// �}�b�v�̏c���ɕ~���}�b�v�`�b�v�̐�
//	constexpr int kMapSizeX = 100;
//	constexpr int kMapSizeY = 20;
//	//constexpr int kMapSizeX = 25;
//	//constexpr int kMapSizeY = 15;
//}

class GameSceneCamera;

class Map
{
private:
	int _mapGraphHandle;

	std::weak_ptr<GameSceneCamera> _camera;

	std::shared_ptr<MapChip> _mapChip[MapGraphData::kMapSizeY][MapGraphData::kMapSizeX];

	//std::shared_ptr<GameSceneCamera> _camera;

public:
	Map();

	/// <summary>
	/// ������
	/// </summary>
	void Init(int mapGraphHandle, std::weak_ptr<GameSceneCamera> camera);
	//void Init(int mapGraphHandle, std::shared_ptr<GameSceneCamera> camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(GameSceneCamera camera);
	//void Draw();

	/// <summary>
	/// �}�b�v�͈͓̔��Ȃ�Ԃ��A�͈͊O�Ȃ�nullptr��Ԃ�
	/// </summary>
	/// <param name="chipX">���ԍ�</param>
	/// <param name="chipY">�c�ԍ�</param>
	/// <returns></returns>
	std::shared_ptr<MapChip> GetMapChipData(int chipX, int chipY)
	{
		// �}�b�v�͈͓̔��Ȃ�Ԃ��A�͈͊O�Ȃ�nullptr��Ԃ�
		if (chipX >= 0 && chipX < MapGraphData::kMapSizeX &&
			chipY >= 0 && chipY < MapGraphData::kMapSizeY)
		{
			return _mapChip[chipY][chipX];
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// pos��vel�����������}�b�v�`�b�v�Ƃ̓����蔻����s��
	/// �������Ă��Ȃ����vel�����̂܂�
	/// �������Ă���Γ�����Ȃ��悤��������vel��Ԃ�
	/// �y�ʉ��̗]�n����
	/// �����ňړ����ă}�b�v�˂��������ꍇ�͍l�����Ă��Ȃ�
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vel"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	Vector2 CheckHitAllMapChip(const Vector2 pos, const Vector2 vel, const Game::Size size);

	/// <summary>
	/// �}�b�v�`�b�v�P�̂Ƃ̓����蔻��
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size"></param>
	/// <param name="chipX"></param>
	/// <param name="chipY"></param>
	/// <returns></returns>
	bool CheckHitMapChip(const Vector2 pos, const Game::Size size, const int chipX, const int chipY);
};

