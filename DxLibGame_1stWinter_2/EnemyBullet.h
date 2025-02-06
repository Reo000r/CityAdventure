#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;
class Player;

namespace EnemyBulletData
{
	// �U����
	constexpr int kAddDamageAmount = 2;



	constexpr float kSizeMul = 3.0f;

	// �g�p����摜�̃T�C�Y
	constexpr int kGraphWidth = 16;
	constexpr int kGraphHeight = 16;

	// �����蔻��̕��ƍ���
	constexpr int kColWidth = static_cast<int>(16 * kSizeMul);
	constexpr int kColHeight = static_cast<int>(16 * kSizeMul);

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 0;

	// �����ʒu�̕␳�l
	// �v���C���[�̔����񎝂��Ă�����
	constexpr int kActivePosOffsetX = 24 / 2 + 12;
	constexpr int kActivePosOffsetY = -8;
	// �����ʒu�̕␳�l
	const Vector2 kActivePosOffset = { kActivePosOffsetX,kActivePosOffsetY };

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 4;

	// �A�j���[�V�����̃R�}��
	constexpr int kAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 2.0f * kSizeMul;

	// ��������
	constexpr int kLifeTime = 120;
}

class EnemyBullet : public BoxCollider
{
private:
	int _graphHandle;
	int _animFrameCount;
	// ���Ȃ�true�A�E�Ȃ�false
	bool _isReverseGraphX;
	// 0�ɂȂ�����񊈐���
	int _lifeTimeCount;

	// true�Ȃ犈����
	// Update��Draw��DisActive���󂯕t����
	// false�Ȃ�񊈐���
	// Active���󂯕t����
	bool _isActive;

	// _pos�͒��S
	Vector2 _vel;
	Vector2 _dir;
	// �`�掞�̕␳�l(�Œ�l)
	// �O���t�B�b�N�̂���𒼂��ׂ̂���
	// ����3/6�h�b�g����Ă���
	Vector2 _drawPosOffset;

	std::weak_ptr<Player> _player;

	bool IsHitPlayer();

public:

	EnemyBullet(int graphHandle);
	~EnemyBullet();

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
	/// ������
	/// </summary>
	bool Active(Vector2 pos, bool isReverse);

	/// <summary>
	/// �񊈐���
	/// </summary>
	void DisActive();

	/// <summary>
	/// ��������Ԃ��擾
	/// </summary>
	bool GetActiveStats() const { return _isActive; }

	/// <summary>
	/// �U���͂��擾
	/// </summary>
	/// <returns></returns>
	int GetDamage() const { return EnemyBulletData::kAddDamageAmount; }
};

