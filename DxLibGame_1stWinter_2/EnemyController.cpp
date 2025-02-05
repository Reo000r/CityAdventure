#include "EnemyController.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "PlayerBulletController.h"

#include <cassert>

EnemyController::EnemyController() : 
	_walkEnemyIdleGraphHandle(0),
	_walkEnemyRunGraphHandle(0), 
	_walkEnemyFallGraphHandle(0),
	_flyEnemyIdleGraphHandle(0),
	_flyEnemyRunGraphHandle(0),
	_flyEnemyDeathGraphHandle(0)
{
}

EnemyController::~EnemyController()
{
	DeleteGraph(_walkEnemyIdleGraphHandle);
	DeleteGraph(_walkEnemyRunGraphHandle);
	DeleteGraph(_walkEnemyFallGraphHandle);
	DeleteGraph(_flyEnemyIdleGraphHandle);
	DeleteGraph(_flyEnemyRunGraphHandle);
	DeleteGraph(_flyEnemyDeathGraphHandle);
}

void EnemyController::Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController)
{
	_map = map;
	_player = player;
	_playerBulletController = playerBulletController;

	// 敵のグラフィックをロード
	_walkEnemyIdleGraphHandle = LoadGraph(L"data/img/enemy/walkenemy/WalkEnemy_Idle.png");
	assert(_walkEnemyIdleGraphHandle >= 0);
	_walkEnemyRunGraphHandle = LoadGraph(L"data/img/enemy/walkenemy/WalkEnemy_Run.png");
	assert(_walkEnemyRunGraphHandle >= 0);
	_walkEnemyFallGraphHandle = LoadGraph(L"data/img/enemy/walkenemy/WalkEnemy_Fall.png");
	assert(_walkEnemyFallGraphHandle >= 0);

	_flyEnemyIdleGraphHandle = LoadGraph(L"data/img/enemy/flyenemy/FlyEnemy_Idle.png");
	assert(_flyEnemyIdleGraphHandle >= 0);
	_flyEnemyRunGraphHandle = LoadGraph(L"data/img/enemy/flyenemy/FlyEnemy_Run.png");
	assert(_flyEnemyRunGraphHandle >= 0);
	_flyEnemyDeathGraphHandle = LoadGraph(L"data/img/enemy/flyenemy/FlyEnemy_Death.png");
	assert(_flyEnemyDeathGraphHandle >= 0);
	
	// 敵を登録されている初期位置の数だけ生成
	for (auto& enemyData : _walkEnemyData)
	{
		// マップチップ番号に合わせているので実際の座標にする
		int mapSize = 32;
		float mapMul = 2.0f;
		enemyData.pos *= mapSize * mapMul;
		// 補正
		enemyData.pos.y += 4 * mapMul;
		
		// 生成と初期化、リストへの追加を行う
		auto enemy = std::make_shared<Enemy01>(enemyData.pos);
		enemy->Init(_map, _player, _playerBulletController, _walkEnemyIdleGraphHandle, _walkEnemyRunGraphHandle, _walkEnemyFallGraphHandle);
		enemy->Active(enemyData.pos, enemyData.isReverse);
		_walkEnemyList.push_back(enemy);
	}
	// 敵を登録されている初期位置の数だけ生成
	for (auto& enemyData : _flyEnemyData)
	{
		// マップチップ番号に合わせているので実際の座標にする
		int mapSize = 32;
		float mapMul = 2.0f;
		enemyData.pos *= mapSize * mapMul;
		// 補正
		//enemyData.pos.y += 4 * mapMul;

		// 生成と初期化、リストへの追加を行う
		auto enemy = std::make_shared<Enemy02>(enemyData.pos);
		enemy->Init(_map, _player, _playerBulletController, _flyEnemyIdleGraphHandle, _flyEnemyRunGraphHandle, _flyEnemyDeathGraphHandle);
		enemy->Active(enemyData.pos, enemyData.isReverse);
		_flyEnemyList.push_back(enemy);
	}
}

void EnemyController::Update()
{
	for (auto& enemy : _walkEnemyList)
	{
		// 活性化状態の敵のみ実行
		if (!enemy->GetActiveStats()) continue;
		enemy->Update();
	}
	for (auto& enemy : _flyEnemyList)
	{
		// 活性化状態の敵のみ実行
		if (!enemy->GetActiveStats()) continue;
		enemy->Update();
	}
}

void EnemyController::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	for (auto& enemy : _walkEnemyList)
	{
		// 活性化状態の敵のみ実行
		if (!enemy->GetActiveStats()) continue;
		enemy->Draw(camera);
	}
	for (auto& enemy : _flyEnemyList)
	{
		// 活性化状態の敵のみ実行
		if (!enemy->GetActiveStats()) continue;
		enemy->Draw(camera);
	}
}
