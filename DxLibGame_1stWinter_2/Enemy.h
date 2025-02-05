#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;
class GameSceneCamera;
class Player;
class PlayerBulletController;

/// <summary>
/// �G�̊��N���X
/// </summary>
class Enemy : public BoxCollider
{
protected:
	int _animFrameCount;

	// �ϋv��
	int _hitPoint;
	// ���Ȃ�true�A�E�Ȃ�false
	bool _isReverseGraphX;

	// true�Ȃ犈����
	// Update��Draw��DisActive���󂯕t����
	// false�Ȃ�񊈐���
	// Active���󂯕t����
	bool _isActive;

	// _pos�͒��S
	Vector2 _vel;

	// �����ʒu�ۑ�
	const Vector2 _spawnPos;

	std::weak_ptr<Map> _map;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">���S�ʒu</param>
	/// <param name="offset">�����蔻��쐬���̈ʒu�␳�l</param>
	/// <param name="size">�\���{�����܂ޔ���̕��ƍ���</param>
	Enemy(Vector2 pos, Vector2 offset, Game::Size size, int hitPoint);

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init(std::weak_ptr<Map> map, 
		std::weak_ptr<Player> player, 
		std::weak_ptr<PlayerBulletController> playerBulletController, 
		int idleHandle, 
		int runHandle, 
		int fallHandle) abstract;

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// �`��S��
	/// </summary>
	virtual void Draw(std::weak_ptr<GameSceneCamera> camera) abstract;

	/// <summary>
	/// ������
	/// </summary>
	virtual bool Active(Vector2 pos, bool isReverse) abstract;

	/// <summary>
	/// �񊈐���
	/// </summary>
	virtual void DisActive() abstract;

	/// <summary>
	/// �_���[�W��H��������̏���
	/// </summary>
	virtual void OnDamage(int damage, bool isReverseX) abstract;

	/// <summary>
	/// ��������Ԃ��擾
	/// </summary>
	bool GetActiveStats() const { return _isActive; }
};
