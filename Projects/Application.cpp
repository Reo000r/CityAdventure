#include "Application.h"
#include "SceneController.h"

#include "Game.h"
#include "Input.h"

#include <DxLib.h>
#include <cassert>
#include <string>

Application& Application::GetInstance()
{
	// この宣言の時点でメモリが確保されてアプリ終了まで残る
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
		// 今回のループが始まった時間を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		ClearDrawScreen();

		// 入力更新
		input.Update();

		// ゲーム部分
		sceneController.Update();
		sceneController.Draw();

		ScreenFlip();
		_frame++;

		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}
}

void Application::Terminate()
{
	DxLib_End();
}
