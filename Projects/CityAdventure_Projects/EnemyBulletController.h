#pragma once
#include "Vector2.h"
#include "Game.h"

#include <vector>
#include <memory>

class EnemyBullet;
class Player;
class Map;
class GameSceneCamera;

namespace EnemyBulletControllerData
{
	// �e�̍ő吔
	constexpr int kBulletNum = 8;
}

/// <summary>
/// �v���C���[�̒e���Ǘ�����N���X
/// </summary>
class EnemyBulletController
{
private:

	int _bulletGraphHandle;

	// �S�Ă̒e���Ǘ�
	std::vector<std::shared_ptr<EnemyBullet>> _bulletList;

	std::weak_ptr<Player> _player;

public:

	EnemyBulletController();
	~EnemyBulletController();

	/// <summary>
	/// ������
	/// </summary>
	void Init(std::weak_ptr<Player> player);

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update(std::weak_ptr<Map> map);

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

	/// <summary>
	/// �����𖞂����Ă�����e��ǉ�(����)����
	/// </summary>
	void AddBullet(Vector2 pos, bool isReverse);

	/// <summary>
	/// �e�Ɠ����������ǂ���
	/// </summary>
	/// <param name="rect"></param>
	/// <param name="returnBullet"></param>
	/// <returns>����������true</returns>
	bool IsHitBullet(Game::Rect rect, std::weak_ptr<EnemyBullet>& returnBullet);

};

