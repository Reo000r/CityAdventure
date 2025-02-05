#pragma once
#include "vector2.h"

#include <memory>

/// <summary>
/// UI�̊��N���X
/// </summary>
class UIBase
{
protected:

	int _useHandle;

	int _animFrameCount;

	// ���Ȃ�true�A�E�Ȃ�false
	bool _isReverseGraphX;

	// true�Ȃ犈����
	// Update��Draw��DisActive���󂯕t����
	// false�Ȃ�񊈐���
	// Active���󂯕t����
	// ����false
	bool _isActive;

	// �����x
	// 0~1
	// 1���s����
	float _alpha;

	// _pos�͒��S
	Vector2 _pos;
	
	Vector2 _vel;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIBase(Vector2 pos);

	// (������������Ȃ��̂�)
	/// <summary>
	/// ������
	/// </summary>
	//virtual void Init(int graphHandle) abstract;

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// �`��S��
	/// </summary>
	virtual void Draw() abstract;

	/// <summary>
	/// ������
	/// </summary>
	virtual bool Active(Vector2 pos, bool isReverse) abstract;

	/// <summary>
	/// �񊈐���
	/// </summary>
	virtual void DisActive() abstract;

	/// <summary>
	/// ��������Ԃ��擾
	/// </summary>
	bool GetActiveStats() const { return _isActive; }
};