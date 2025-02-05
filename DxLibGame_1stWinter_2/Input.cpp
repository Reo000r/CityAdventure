#include "Input.h"

#include <DxLib.h>

Input& Input::GetInstance()
{
	// この宣言の時点でメモリが確保されてアプリ終了まで残る
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
	// 押されていればtrue
	return (_padInput & button);
}

bool Input::IsTrigger(int button) const
{
	// 押されていればtrue.そうでないならfalse
	bool isNow = (_padInput & button);
	bool isLast = (_lastInput & button);
	return (isNow && !isLast);
}
