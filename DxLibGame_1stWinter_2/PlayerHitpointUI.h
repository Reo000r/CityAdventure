#pragma once
#include "UIBase.h"
#include "Vector2.h"

class Player;

namespace PlayerHitpointUIData
{
	constexpr float kUndamageSizeMul = 3.0f;
	constexpr float kDamageSizeMul   = 2.0f;

	// �O���t�B�b�N�̕��ƍ���
	constexpr int kUndamageGraphWidth  = 16;
	constexpr int kUndamageGraphHeight = 16;
	constexpr int kDamageGraphWidth  = 16;
	constexpr int kDamageGraphHeight = 16;

	// �`�掞�̕␳�l
	constexpr int kUndamageDrawPosOffsetX = 0;
	constexpr int kUndamageDrawPosOffsetY = 0;
	constexpr int kDamageDrawPosOffsetX = 0;
	constexpr int kDamageDrawPosOffsetY = 0;
	const Vector2 kUndamageDrawPosOffset = 
	{ kUndamageDrawPosOffsetX, kUndamageDrawPosOffsetY };
	const Vector2 kDamageDrawPosOffset = 
	{ kDamageDrawPosOffsetX, kDamageDrawPosOffsetY };

	// �`��{�����܂񂾕��ƍ���
	//constexpr int kWidth = kGraphWidth * kSizeMul;
	//constexpr int kHeight = kGraphHeight * kSizeMul;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kUndamageSingleAnimFrame = 10;
	constexpr int kDamageSingleAnimFrame = 30;
	// 
	constexpr int kDamageEffectAnimFrame = 60;

	// �A�j���[�V�����̃R�}��
	constexpr int kUndamageAnimNum = 4;
	constexpr int kDamageAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kUndamageAnimTotalFrame = kUndamageAnimNum * kUndamageSingleAnimFrame;
	constexpr int kDamageAnimTotalFrame = kDamageAnimNum * kDamageSingleAnimFrame;
}

/// <summary>
/// HP��ЂƂ�
/// </summary>
class PlayerHitpointUI : public UIBase
{
private:

	int _undamagedGraphHandle;
	int _damagedGraphHandle;

	// ���������Ԗڂ�HPUI�Ȃ̂��ۑ�
	// 1����n�߂�
	int _hitpointIndex;

	// HP�̍ő�l�ƌ��ݒl��ۑ�
	int _playerMaxHitpoint;
	int _playerCurrentHitpoint;
	
	int _damageEffectCount;

	Vector2 _drawPosOffset;

	std::weak_ptr<Player> _player;

	using UpdateFunc_t = void(PlayerHitpointUI::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// �_���[�W��H�炤�O
	/// </summary>
	void UndamageUpdate();
	
	/// <summary>
	/// �_���[�W��H������u��
	/// </summary>
	void OndamageUpdate();
	
	/// <summary>
	/// �_���[�W��H�������
	/// </summary>
	void DamageUpdate();

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayerHitpointUI(Vector2 pos, int hitpointIndex);

	/// <summary>
	/// ������
	/// </summary>
	void Init(std::weak_ptr<Player> player, int undamagedGraphHandle, int damagedGraphHandle);

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ������
	/// </summary>
	bool Active(Vector2 pos, bool isReverse) override;

	/// <summary>
	/// �񊈐���
	/// </summary>
	void DisActive() override;
};

