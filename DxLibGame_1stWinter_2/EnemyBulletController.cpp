#include "EnemyBulletController.h"
#include "EnemyBullet.h"
#include "Map.h"
#include "GameSceneCamera.h"

#include <cassert>

EnemyBulletController::EnemyBulletController() :
	_bulletGraphHandle(0)
{
}

EnemyBulletController::~EnemyBulletController()
{
	DeleteGraph(_bulletGraphHandle);
}

void EnemyBulletController::Init(std::weak_ptr<Player> player)
{
	_player = player;

	// �e�̃O���t�B�b�N�����[�h
	_bulletGraphHandle = LoadGraph(L"data/img/enemy/bossenemy/BossEnemy_Bullet.png");
	assert(_bulletGraphHandle >= 0);

	// �e��萔�̐���������
	for (int i = 0; i < EnemyBulletControllerData::kBulletNum; i++)
	{
		// �����Ə������A���X�g�ւ̒ǉ����s��
		// std::shared_ptr<PlayerBullet>
		auto bullet = std::make_shared<EnemyBullet>(_bulletGraphHandle);
		bullet->Init(_player);
		_bulletList.push_back(bullet);
	}
}

void EnemyBulletController::Update(std::weak_ptr<Map> map)
{
	for (auto& bullet : _bulletList)
	{
		// �񊈐�����ԂȂ��΂�
		if (!bullet->GetActiveStats()) continue;

		bullet->Update(map);
	}
}

void EnemyBulletController::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	for (auto& bullet : _bulletList)
	{
		// �񊈐�����ԂȂ��΂�
		if (!bullet->GetActiveStats()) continue;

		bullet->Draw(camera);
	}
}

void EnemyBulletController::AddBullet(Vector2 pos, bool isReverse)
{
	// �񊈐������Ă�����̂������������������
	// �֐����Ă�true �Ȃ琬��(�������ł���)
	//           false�Ȃ玸�s(��������Ԃ��������G���[)
	for (auto& bullet : _bulletList)
	{
		// �e�𔭎˂ł����ꍇ�͏I��
		if (bullet->Active(pos, isReverse))
		{
			break;
		}
	}
}

bool EnemyBulletController::IsHitBullet(Game::Rect rect, std::weak_ptr<EnemyBullet>& returnBullet)
{
	// �S�Ă̒e�Ɠ����蔻����s��
	for (auto& bullet : _bulletList)
	{
		// �񊈐�����ԂȂ��΂�
		if (!bullet->GetActiveStats()) continue;

		// �e�̓����蔻����擾
		Game::Rect bulletRect = bullet->GetRect();

		// �e�Ɠ����蔻��
		bool isHit = !(
			(rect.top >= bulletRect.bottom || rect.bottom <= bulletRect.top) ||
			(rect.left >= bulletRect.right || rect.right <= bulletRect.left));
		// �������Ă�����
		if (isHit)
		{
			returnBullet = bullet;
			// �e��񊈐���
			bullet->DisActive();
			return true;
		}
	}

	// �������Ă��Ȃ�������
	return false;
}