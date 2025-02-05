#include "Input.h"

#include <DxLib.h>

Input& Input::GetInstance()
{
	// ���̐錾�̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
	static Input input;
	return input;
}

void Input::Update()
{
	_lastInput = _padInput;

	_padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

bool Input::IsPress(int button) const
{
	// ������Ă����true
	return (_padInput & button);
}

bool Input::IsTrigger(int button) const
{
	// ������Ă����true.�����łȂ��Ȃ�false
	bool isNow = (_padInput & button);
	bool isLast = (_lastInput & button);
	return (isNow && !isLast);
}
