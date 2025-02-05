#include "Application.h"
#include "SceneController.h"

#include "Game.h"
#include "Input.h"

#include <DxLib.h>
#include <cassert>
#include <string>

Application& Application::GetInstance()
{
	// ���̐錾�̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
	static Application app;
	return app;
}

bool Application::Init()
{
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, 32);
	ChangeWindowMode(false);
	SetWindowText(L"ShotMan MainWindow");

	if (DxLib_Init())
	{
		return false;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

void Application::Run()
{
	_frame = 0;
	SceneController& sceneController = SceneController::GetInstance();
	Input& input = Input::GetInstance();

	while (ProcessMessage() == 0)
	{
		// ����̃��[�v���n�܂������Ԃ��o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		ClearDrawScreen();

		// ���͍X�V
		input.Update();

		// �Q�[������
		sceneController.Update();
		sceneController.Draw();

		ScreenFlip();
		_frame++;

		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}
}

void Application::Terminate()
{
	DxLib_End();
}
