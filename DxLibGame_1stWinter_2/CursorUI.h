#pragma once
#include "Vector2.h"

namespace CursorUIData
{
	//constexpr float kCursorSizeMul = 3.0f;
	//constexpr float kEffectSizeMul = 3.0f;

	// �O���t�B�b�N�̕��ƍ���
	constexpr int kUndecisionGraphWidth  = 48;
	constexpr int kUndecisionGraphHeight = 48;
	constexpr int kDecisionGraphWidth  = 48;
	constexpr int kDecisionGraphHeight = 48;
	constexpr int kEffectGraphWidth  = 16;
	constexpr int kEffectGraphHeight = 16;

	// �`�掞�̕␳�l
	constexpr int kUndecisionDrawPosOffsetX = 0;
	constexpr int kUndecisionDrawPosOffsetY = 0;
	constexpr int kDecisionDrawPosOffsetX = 0;
	constexpr int kDecisionDrawPosOffsetY = 0;
	constexpr int kEffectDrawPosOffsetX = 0;
	constexpr int kEffectDrawPosOffsetY = -6;
	const Vector2 kUndecisionDrawPosOffset =
	{ kUndecisionDrawPosOffsetX, kUndecisionDrawPosOffsetY };
	const Vector2 kDecisionDrawPosOffset =
	{ kDecisionDrawPosOffsetX, kDecisionDrawPosOffsetY };
	
	// �G�t�F�N�g���J�[�\������ǂ̂��炢���炵�ĕ\�����邩
	constexpr int kEffectPosOffsetX = 28;
	constexpr int kEffectPosOffsetY = 0 ;
	

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kUndecisionSingleAnimFrame = 15;
	constexpr int kDecisionSingleAnimFrame   = 6;
	constexpr int kEffectSingleAnimFrame = 5;
		
	// �A�j���[�V�����̃R�}��
	constexpr int kUndecisionAnimNum = 5;
	constexpr int kDecisionAnimNum   = 6;
	constexpr int kEffectAnimNum = 4;

	// �A�j���[�V�������������̃t���[����
	constexpr int kUndecisionAnimTotalFrame = kUndecisionAnimNum * kUndecisionSingleAnimFrame;
	constexpr int kDecisionAnimTotalFrame   = kDecisionAnimNum   * kDecisionSingleAnimFrame;
	constexpr int kEffectAnimTotalFrame = kEffectAnimNum * kEffectSingleAnimFrame;
}

/// <summary>
/// �ǂ̑I������I��ł��邩������₷�����邽�߂̂���
/// </summary>
class CursorUI
{
	int _useHandle;

	int _cursorUndecisionHandle;
	int _cursorDecisionHandle;
	int _effectHandle;

	//�A�j���[�V����1�R�}�̃t���[����
	int _undecisionSingleAnimFrame;
	int _decisionSingleAnimFrame;
	int _effectSingleAnimFrame;
	int _undecisionAnimTotalFrame;
	int _decisionAnimTotalFrame;
	int _effectAnimTotalFrame;

	int _cursorAnimFrameCount;
	int _effectAnimFrameCount;
	int _decisionFrameCount;

	float _cursorSizeMul;
	float _effectSizeMul;

	// true�Ȃ���͂�����
	bool _canInput;
	bool _isActiveEffect;

	// ���ݑI�𒆂̏ꏊ
	// 0:�� 1:��
	// (enum����scene������̂���ς�������)
	int _current;

	// _pos�͒��S
	Vector2 _cursorPos;
	Vector2 _effectPos;
	Vector2 _cursorVel;
	Vector2 _effectVel;
	Vector2 _addPos;
	Vector2 _drawPos;
	Vector2 _cursorDrawPosOffset;

	// *_effectSizeMul����
	Vector2 _effectDrawPosOffset;
	Vector2 _effectPosOffset;

	using UpdateFunc_t = void(CursorUI::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// �����莞��Update
	/// </summary>
	void UndecisionUpdate();

	/// <summary>
	/// ���莞��Update
	/// </summary>
	void DecisionUpdate();

	/// <summary>
	/// EffectGraph�݂̂�Update
	/// ����Update����Ă�ł���
	/// ���O����𐶂݂���
	/// </summary>
	void EffectUpdate();

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CursorUI();

	/// <summary>
	/// ������
	/// </summary>
	void Init(Vector2 pos, Vector2 addPos, float cursorSizeMul, float effectSizeMul, int cursorUndecisionHandle, int cursorDecisionHandle, int effectHandle, bool canInput);

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��S��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ړ��ݒ�
	/// </summary>
	/// <param name="vel"></param>
	void SetCursorVel(Vector2 vel) { _cursorVel = vel; }

	/// <summary>
	/// �ړ��ݒ�
	/// </summary>
	/// <param name="vel"></param>
	void SetEffectVel(Vector2 vel) { _effectVel = vel; }

	/// <summary>
	/// ���݂̃O���t�B�b�N������������
	/// ���\����...
	/// </summary>
	/// <param name="graphHandle"></param>
	void SetUseHandle(int graphHandle) { _useHandle = graphHandle; }

	/// <summary>
	/// ���͎�t��Ԃ�؂�ւ���
	/// </summary>
	/// <param name="canInput"></param>
	void SetCanInput(bool canInput) { _canInput = canInput; }

	/// <summary>
	/// _undecisionSingleAnimFrame
	/// _decisionSingleAnimFrame
	/// _effectSingleAnimFrame
	/// </summary>
	void SetSingleAnimFrame(int und, int d, int e)
	{
		_undecisionSingleAnimFrame = und;
		_decisionSingleAnimFrame = d;
		_effectSingleAnimFrame = e;
		_undecisionAnimTotalFrame = CursorUIData::kUndecisionAnimNum * _undecisionSingleAnimFrame;
		_decisionAnimTotalFrame = CursorUIData::kDecisionAnimNum * _decisionSingleAnimFrame;
		_effectAnimTotalFrame = CursorUIData::kEffectAnimNum * _effectSingleAnimFrame;
	}

	/// <summary>
	/// ���肪������Ă���̌o�߃t���[��
	/// </summary>
	/// <returns></returns>
	int GetDecisionFrame() const { return _decisionFrameCount; }

	/// <summary>
	/// ���ݑI�𒆂̏ꏊ
	/// 0:�� 1:��
	/// (enum����scene������̂���ς�������)
	/// </summary>
	/// <returns></returns>
	int GetCurrentPosition() const { return _current; }
};