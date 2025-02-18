#pragma once
#include "Game.h"
#include "MapChip.h"
#include <memory>

class GameSceneCamera;

/// <summary>
/// MapChip���Ǘ�����
/// �V�[��������shared_ptr�������A����weak_ptr���������Ȃ�
/// </summary>
class Map
{
private:
	int _mapGraphHandle;

	// �����蔻����s���ۂɕK�v�Ȃ̂Ŏ���
	std::weak_ptr<GameSceneCamera> _camera;

	std::shared_ptr<MapChip> _mapChip[MapGraphData::kMapSizeY][MapGraphData::kMapSizeX];

public:
	Map();

	/// <summary>
	/// ������
	/// </summary>
	void Init(int mapGraphHandle, std::weak_ptr<GameSceneCamera> camera);
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() const;

	/// <summary>
	/// �}�b�v�͈͓̔��Ȃ�Ԃ��A�͈͊O�Ȃ�nullptr��Ԃ�
	/// </summary>
	/// <param name="chipX">���ԍ�</param>
	/// <param name="chipY">�c�ԍ�</param>
	std::shared_ptr<MapChip> GetMapChipData(int chipX, int chipY) const
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
	Vector2 CheckHitAllMapChip(const Vector2 pos, const Vector2 vel, const Game::Size size) const;

	/// <summary>
	/// �}�b�v�`�b�v�P�̂Ƃ̓����蔻��
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="size"></param>
	/// <param name="chipX"></param>
	/// <param name="chipY"></param>
	bool CheckHitMapChip(const Vector2 pos, const Game::Size size, const int chipX, const int chipY) const;
};

