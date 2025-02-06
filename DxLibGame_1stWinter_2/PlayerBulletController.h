#pragma once
#include "Vector2.h"
#include "Game.h"
//#include "PlayerBullet.h"

#include <vector>
#include <memory>

class PlayerBullet;
class Map;
class GameSceneCamera;

namespace
{
	// �e�̍ő吔
	constexpr int kBulletNum = 6;
}

/// <summary>
/// �v���C���[�̒e���Ǘ�����N���X
/// </summary>
class PlayerBulletController
{
private:

	int _bulletGraphHandle;

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
	bool IsHitBullet(Game::Rect rect, std::weak_ptr<PlayerBullet>& returnBullet);

};

