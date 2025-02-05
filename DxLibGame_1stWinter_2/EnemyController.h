#pragma once
#include "Vector2.h"

#include <vector>
#include <memory>

class Enemy01;
class Enemy02;
class Map;
class GameSceneCamera;
class Player;
class PlayerBulletController;

/// <summary>
/// Enemy‚ÌUpdate‚âDraw‚ğŠÇ—‚·‚éƒNƒ‰ƒX
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

	// •à‚­“G‚Ì‰ŠúÀ•W
	// ‚à‚¤‚¿‚å‚Á‚Æ—Ç‚¢•û–@‚Å¶¬‚³‚¹‚½‚¢
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
	};

	// 1 x11ˆÈ‰º‚Ífalse
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
	};

	// •à‚­“G‚ğŠÇ—
	std::vector<std::shared_ptr<Enemy01>> _walkEnemyList;
	// •‚—V“G‚ğŠÇ—
	std::vector<std::shared_ptr<Enemy02>> _flyEnemyList;

	std::weak_ptr<Map> _map;

	std::weak_ptr<Player> _player;
	std::weak_ptr<PlayerBulletController> _playerBulletController;

public:

	EnemyController();
	~EnemyController();

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController);

	/// <summary>
	/// “à•”•Ï”‚ÌXV
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ‘S”Ê
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

	///// <summary>
	///// ğŒ‚ğ–‚½‚µ‚Ä‚¢‚½‚çw’èÀ•W‚É“G‚ğ’Ç‰Á‚·‚é
	///// </summary>
	//void AddBullet(Vector2 pos, bool isReverse);
};

