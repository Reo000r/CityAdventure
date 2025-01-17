#pragma once
#include "Vector2.h"
//#include "PlayerBullet.h"

#include <vector>
#include <memory>

class PlayerBullet;
class Map;
class GameSceneCamera;

namespace
{
	constexpr int kBulletNum = 50;
}

class PlayerBulletController
{
private:

	// �S�Ă̒e���Ǘ�
	std::vector<std::shared_ptr<PlayerBullet>> _bulletList;

public:

	PlayerBulletController();
	~PlayerBulletController();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update(std::shared_ptr<Map> map);

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(GameSceneCamera camera);

	/// <summary>
	/// �����𖞂����Ă�����e��ǉ�(����)����
	/// </summary>
	void AddBullet(Vector2 pos, bool isReverse);

};

