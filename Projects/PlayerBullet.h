#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;

namespace PlayerBulletData
{
	// �U����
	constexpr int kAddDamageAmount = 1;



	constexpr float kSizeMul = 2.0f;

	// �g�p����摜�̃T�C�Y
	constexpr int kGraphWidth  = 16;
	constexpr int kGraphHeight = 16;
	
	// �����蔻��̕��ƍ���
	constexpr int kColWidth  = static_cast<int>(16 * kSizeMul);
	constexpr int kColHeight = static_cast<int>(6 * kSizeMul);

	// �`�掞�̕␳�l
	constexpr int kDrawPosOffsetX = 0;
	constexpr int kDrawPosOffsetY = 6;

	// �����ʒu�̕␳�l
	constexpr int kActivePosOffsetX = 24 / 2 + 12;
	constexpr int kActivePosOffsetY = -8;
	// �����ʒu�̕␳�l
	const Vector2 kActivePosOffset = { kActivePosOffsetX,kActivePosOffsetY };

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 2;

	// �A�j���[�V�����̃R�}��
	constexpr int kAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kAnimTotalFrame = kAnimNum * kSingleAnimFrame;

	// �ړ����x
	constexpr float kMoveSpeed = 4.5f * kSizeMul;

	// ��������
	constexpr int kLifeTime = 90;
}

class PlayerBullet : public BoxCollider
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

	Vector2 _vel;
	// �`�掞�̕␳�l(�Œ�l)
	// �O���t�B�b�N�̂���𒼂��ׂ̂���
	Vector2 _drawPosOffset;

public:

	PlayerBullet(int graphHandle);
	~PlayerBullet();

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
	int GetDamage() const { return PlayerBulletData::kAddDamageAmount; }
};

