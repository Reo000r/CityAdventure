#pragma once

#include "Vector2.h"

#include <memory>
#include <vector>

class Player;
class PlayerHitpointUI;
class GameSceneCamera;

/// <summary>
/// UIを管理する
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

	// UI達


	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::weak_ptr<Player> player);

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);
};