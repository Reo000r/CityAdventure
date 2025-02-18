#pragma once

#include "Vector2.h"

#include <memory>
#include <vector>

class Player;
class PlayerHitpointUI;
class GameSceneCamera;

/// <summary>
/// UI���Ǘ�����
/// </summary>
class UIController
{
private:

	int _playerUndamagedHitpointGraphHandle;
	int _playerDamagedHitpointGraphHandle;
	
	std::weak_ptr<Player> _player;
	std::vector<std::shared_ptr<PlayerHitpointUI>> _hpList;

public:

	UIController();
	~UIController();

	// UI�B


	/// <summary>
	/// ������
	/// </summary>
	void Init(std::weak_ptr<Player> player);

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);
};