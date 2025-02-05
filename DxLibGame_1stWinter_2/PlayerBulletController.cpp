#include "PlayerBulletController.h"
#include "PlayerBullet.h"
#include "Map.h"
#include "GameSceneCamera.h"

#include <cassert>

PlayerBulletController::PlayerBulletController() :
	_bulletGraphHandle(0)
{
}

PlayerBulletController::~PlayerBulletController()
{
	DeleteGraph(_bulletGraphHandle);
}

void PlayerBulletController::Init()
{
	// �e�̃O���t�B�b�N�����[�h
	_bulletGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Bullet.png");
	assert(_bulletGraphHandle >= 0);

	// �e��萔�̐���������
	for (int i = 0; i < kBulletNum; i++)
	{
		// �����Ə������A���X�g�ւ̒ǉ����s��
		// std::shared_ptr<PlayerBullet>
		auto bullet = std::make_shared<PlayerBullet>(_bulletGraphHandle);
		bullet->Init();
		_bulletList.push_back(bullet);
	}
}

void PlayerBulletController::Update(std::weak_ptr<Map> map)
{
	for (auto& bullet : _bulletList)
	{
		// �񊈐�����ԂȂ��΂�
		if (!bullet->GetActiveStats()) continue;
		
		bullet->Update(map);
	}
}

void PlayerBulletController::Draw(std::weak_ptr<GameSceneCamera> camera)
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

bool PlayerBulletController::IsHitBullet(Game::Rect rect, std::weak_ptr<PlayerBullet>& returnBullet)
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