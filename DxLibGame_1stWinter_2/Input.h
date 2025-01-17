#pragma once

namespace
{
	// ���͏��
	// ����Player�����u���������Ă��Ȃ��͂�
#define INPUTRIGHT PAD_INPUT_RIGHT
#define INPUTLEFT PAD_INPUT_LEFT
#define INPUTJUMP PAD_INPUT_2
#define INPUTSHOT PAD_INPUT_3
}

/// <summary>
/// ���͌n���R���g���[������N���X
/// �V���O���g��������
/// </summary>
class Input
{
private:
	Input() {}
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	int _padInput = 0;
	int _lastInput = 0;

public:
	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>Application�V���O���g���I�u�W�F�N�g</returns>
	static Input& GetInstance();

	/// <summary>
	/// ���͏��̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ������Ă��邩�ǂ����̎擾
	/// </summary>
	/// <param name="button">������s���{�^��</param>
	/// <returns>������Ă����true�A�łȂ����false</returns>
	bool IsPress(int button);

	/// <summary>
	/// �����ꂽ�u�Ԃ��ǂ����̎擾
	/// </summary>
	/// <param name="button">������s���{�^��</param>
	/// <returns>������Ă����true�A�łȂ����false</returns>
	bool IsTrigger(int button);
};