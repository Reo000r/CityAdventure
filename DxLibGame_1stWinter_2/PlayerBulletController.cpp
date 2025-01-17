#include "PlayerBulletController.h"
#include "PlayerBullet.h"
#include "Map.h"
#include "GameSceneCamera.h"

#include <cassert>

PlayerBulletController::PlayerBulletController()
{
}

PlayerBulletController::~PlayerBulletController()
{
}

void PlayerBulletController::Init()
{
	// �e�̃O���t�B�b�N�����[�h
	int bulletGraphHandle = LoadGraph(L"data/img/player/0/Player0_Bullet.png");
	assert(bulletGraphHandle >= 0);

	// �e��萔�̐���������
	for (int i = 0; i < kBulletNum; i++)
	{
		// �����Ə������A���X�g�ւ̒ǉ����s��
		// std::shared_ptr<PlayerBullet>
		auto bullet = std::make_shared<PlayerBullet>(bulletGraphHandle);
		bullet->Init();
		_bulletList.push_back(bullet);
	}
}

void PlayerBulletController::Update(std::shared_ptr<Map> map)
{
	for (auto& bullet : _bulletList)
	{
		// �񊈐�����ԂȂ��΂�
		if (!bullet->GetActiveStats()) continue;
		
		bullet->Update(map);
	}
}

void PlayerBulletController::Draw(GameSceneCamera camera)
{
	for (auto& bullet : _bulletList)
	{
		// �񊈐�����ԂȂ��΂�
		if (!bullet->GetActiveStats()) continue;

		bullet->Draw(camera);
	}
}

void PlayerBulletController::AddBullet(Vector2 pos, bool isReverse)
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
