#include "EnemyController.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "BossEnemy.h"
#include "PlayerBulletController.h"
#include "EnemyBulletController.h"

#include <cassert>

EnemyController::EnemyController() : 
	_walkEnemyIdleGraphHandle(0),
	_walkEnemyRunGraphHandle(0), 
	_walkEnemyFallGraphHandle(0),
	_flyEnemyIdleGraphHandle(0),
	_flyEnemyRunGraphHandle(0),
	_flyEnemyDeathGraphHandle(0),
	_bossEnemyIdleGraphHandle(0),
	_bossEnemyRunGraphHandle(0),
	_bossEnemyDeathGraphHandle(0)
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
	DeleteGraph(_bossEnemyIdleGraphHandle);
	DeleteGraph(_bossEnemyRunGraphHandle);
	DeleteGraph(_bossEnemyDeathGraphHandle);
}

void EnemyController::Init(std::weak_ptr<Map> map, std::weak_ptr<Player> player, std::weak_ptr<PlayerBulletController> playerBulletController,std::weak_ptr<EnemyBulletController> enemyBulletController)
{
	_map = map;
	_player = player;
	_playerBulletController = playerBulletController;
	_enemyBulletController = enemyBulletController;

	// �G�̃O���t�B�b�N�����[�h
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

	_bossEnemyIdleGraphHandle = LoadGraph(L"data/img/enemy/bossenemy/BossEnemy_Idle.png");
	assert(_bossEnemyIdleGraphHandle >= 0);
	_bossEnemyRunGraphHandle = LoadGraph(L"data/img/enemy/bossenemy/BossEnemy_Run.png");
	assert(_bossEnemyRunGraphHandle >= 0);
	_bossEnemyDeathGraphHandle = LoadGraph(L"data/img/enemy/bossenemy/BossEnemy_Death.png");
	assert(_bossEnemyDeathGraphHandle >= 0);
	
	// �G��o�^����Ă��鏉���ʒu�̐���������
	for (auto& enemyData : _walkEnemyData)
	{
		// �}�b�v�`�b�v�ԍ��ɍ��킹�Ă���̂Ŏ��ۂ̍��W�ɂ���
		int mapSize = 32;
		float mapMul = 2.0f;
		enemyData.pos *= mapSize * mapMul;
		// �␳
		enemyData.pos.y += 4 * mapMul;
		
		// �����Ə������A���X�g�ւ̒ǉ����s��
		auto enemy = std::make_shared<Enemy01>(enemyData.pos);
		enemy->Init(_map, _player, _playerBulletController, _walkEnemyIdleGraphHandle, _walkEnemyRunGraphHandle, _walkEnemyFallGraphHandle);
		enemy->Active(enemyData.pos, enemyData.isReverse);
		_walkEnemyList.push_back(enemy);
	}
	// �G��o�^����Ă��鏉���ʒu�̐���������
	for (auto& enemyData : _flyEnemyData)
	{
		// �}�b�v�`�b�v�ԍ��ɍ��킹�Ă���̂Ŏ��ۂ̍��W�ɂ���
		int mapSize = 32;
		float mapMul = 2.0f;
		enemyData.pos *= mapSize * mapMul;

		// �����Ə������A���X�g�ւ̒ǉ����s��
		auto enemy = std::make_shared<Enemy02>(enemyData.pos);
		enemy->Init(_map, _player, _playerBulletController, _flyEnemyIdleGraphHandle, _flyEnemyRunGraphHandle, _flyEnemyDeathGraphHandle);
		enemy->Active(enemyData.pos, enemyData.isReverse);
		_flyEnemyList.push_back(enemy);
	}
	// �G��o�^����Ă��鏉���ʒu�̐���������
	for (auto& enemyData : _bossFlyEnemyData)
	{
		// �}�b�v�`�b�v�ԍ��ɍ��킹�Ă���̂Ŏ��ۂ̍��W�ɂ���
		int mapSize = 32;
		float mapMul = 2.0f;
		enemyData.pos *= mapSize * mapMul;

		// �����Ə������A���X�g�ւ̒ǉ����s��
		auto enemy = std::make_shared<BossEnemy>(enemyData.pos);
		enemy->Init(_map, _player, _playerBulletController, _bossEnemyIdleGraphHandle, _bossEnemyRunGraphHandle, _bossEnemyDeathGraphHandle);
		enemy->Active(enemyData.pos, enemyData.isReverse);
		enemy->SetEnemyBulletController(_enemyBulletController);
		_bossEnemyList.push_back(enemy);
	}
	_enemyBulletController.lock()->Init(_player);
}

void EnemyController::Update()
{
	// ��������Ԃ̓G�̂ݎ��s
	for (auto& enemy : _walkEnemyList)
	{
		if (!enemy->GetActiveStats()) continue;
		enemy->Update();
	}
	for (auto& enemy : _flyEnemyList)
	{
		if (!enemy->GetActiveStats()) continue;
		enemy->Update();
	}
	for (auto& enemy : _bossEnemyList)
	{
		if (!enemy->GetActiveStats()) continue;
		enemy->Update();
	}
	_enemyBulletController.lock()->Update(_map);
}

void EnemyController::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	// ��������Ԃ̓G�̂ݎ��s
	for (auto& enemy : _walkEnemyList)
	{
		if (!enemy->GetActiveStats()) continue;
		enemy->Draw(camera);
	}
	for (auto& enemy : _flyEnemyList)
	{
		if (!enemy->GetActiveStats()) continue;
		enemy->Draw(camera);
	}
	for (auto& enemy : _bossEnemyList)
	{
		if (!enemy->GetActiveStats()) continue;
		enemy->Draw(camera);
	}
	_enemyBulletController.lock()->Draw(camera);
}

bool EnemyController::IsAllBossKill()
{
	bool ans = true;
	for (auto& boss : _bossEnemyList)
	{
		ans = boss->IsDead();
		if (!ans) break;
	}

	return ans;
}
