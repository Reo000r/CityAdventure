#pragma once
#include "Vector2.h"

#include <vector>
#include <memory>

class Enemy01;
class Enemy02;
class BossEnemy;
class Map;
class GameSceneCamera;
class Player;
class PlayerBulletController;
class EnemyBulletController;

/// <summary>
/// EnemyのUpdateやDrawを管理するクラス
/// </summary>
class EnemyController
{
private:

	int _walkEnemyIdleGraphHandle;
	int _walkEnemyRunGraphHandle;
	int _walkEnemyFallGraphHandle;

	int _flyEnemyIdleGraphHandle;
	int _flyEnemyRunGraphHandle;
	int _flyEnemyDeathGraphHandle;

	int _bossEnemyIdleGraphHandle;
	int _bossEnemyRunGraphHandle;
	int _bossEnemyDeathGraphHandle;

	struct WalkEnemyData
	{
		Vector2 pos;
		bool isReverse;
	};

	struct FlyEnemyData
	{
		Vector2 pos;
		bool isReverse;
	};

	struct BossFlyEnemyData
	{
		Vector2 pos;
		bool isReverse;
	};

	// 歩く敵の初期座標
	// もうちょっと良い方法で生成させたい
	std::vector<WalkEnemyData> _walkEnemyData =
	{
		// 1
		{ { 16, 35 }, true },
		{ {  3, 29 }, false },
		{ { 17, 27 }, true },
		{ {  3, 21 }, false },
		{ {  4, 15 }, false },
		{ {  2, 12 }, false },
		{ { 15,  8 }, true },

		// 1-2
		{ { 21,  5 }, true },
		{ { 29,  5 }, true },
		{ { 34,  5 }, true },

		// 2
		{ { 40,  16 }, true },
		{ { 33,  31 }, true },
		{ { 36,  31 }, true },
		{ { 28,  35 }, false },
		{ { 31,  35 }, false },
		{ { 34,  35 }, false },

		// 3,4
		{ { 59,  35 }, true },
		{ { 75,  34 }, true },

		{ { 92,  32 }, true },
		{ { 92,  26 }, true },
		{ { 84,  21 }, false },

		{ { 54,  27 }, false },
		{ { 52,  23 }, false },
		{ { 62,  24 }, true },
		{ { 67,  21 }, true },
		{ { 57,  18 }, false },
		{ { 52,  15 }, false },
		{ { 66,  14 }, true },
		{ { 75,  13 }, true },

		{ { 84,  17 }, true },
		{ { 92,  17 }, true },
		{ { 88,  14 }, true },
	};

	// 1 x11以下はfalse
	std::vector<FlyEnemyData> _flyEnemyData =
	{
		// 1
		{ {  4, 27 }, false },
		{ { 13, 22 }, true },
		{ { 11, 13 }, true },
		{ {  4, 10 }, false },
		
		// 1-2
		{ {  6,  4 }, false },
		{ { 16,  4 }, true },
		{ { 32,  3 }, true },

		// 2
		{ { 41,  9 }, true },
		{ { 29,  9 }, false },
		{ { 27, 17 }, false },
		{ { 36, 15 }, true },
		{ { 39, 14 }, true },
		{ { 30, 22 }, false },
		{ { 28, 24 }, false },
		{ { 35, 26 }, true },
		{ { 38, 26 }, true },
		{ { 41, 26 }, true },
		{ { 41, 33 }, true },
		{ { 32, 34 }, false },

		// 3,4
		{ { 54, 34 }, true },
		{ { 57, 33 }, true },
		{ { 66, 32 }, true },
		{ { 71, 32 }, true },

		{ { 79, 33 }, true },
		{ { 82, 32 }, true },
		{ { 86, 28 }, false },
		{ { 87, 22 }, false },
		{ { 92, 20 }, true },
		{ { 79, 20 }, false },
		{ { 78, 25 }, false },
		{ { 82, 28 }, true },
		{ { 73, 26 }, false },

		{ { 66, 24 }, false },
		{ { 64, 27 }, false },
		{ { 58, 28 }, false },
		{ { 54, 22 }, false },
		{ { 52, 21 }, false },
		{ { 60, 23 }, true },
		{ { 65, 18 }, false },
		{ { 67, 17 }, true },
		{ { 61, 17 }, false },
		{ { 52, 13 }, false },
		{ { 62, 12 }, true },
		{ { 74, 12 }, true },

		{ { 80, 12 }, true },
		{ { 85, 16 }, true },
		{ { 91, 15 }, true },
		{ { 91, 11 }, true },
	};

	// 1 x11以下はfalse
	std::vector<BossFlyEnemyData> _bossFlyEnemyData =
	{  
		{ { 61,  5 }, false },
		{ { 83,  5 }, true },
	};

	// 歩く敵を管理
	std::vector<std::shared_ptr<Enemy01>> _walkEnemyList;
	// 浮遊敵を管理
	std::vector<std::shared_ptr<Enemy02>> _flyEnemyList;
	// ボスを管理
	std::vector<std::shared_ptr<BossEnemy>> _bossEnemyList;

	std::weak_ptr<Map> _map;

	std::weak_ptr<Player> _player;
	std::weak_ptr<PlayerBulletController> _playerBulletController;
	std::weak_ptr<EnemyBulletController> _enemyBulletController;

public:

	EnemyController();
	~EnemyController();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::weak_ptr<Map> map, 
		std::weak_ptr<Player> player, 
		std::weak_ptr<PlayerBulletController> playerBulletController, 
		std::weak_ptr<EnemyBulletController> enemyBulletController);

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

	bool IsAllBossKill();
};

