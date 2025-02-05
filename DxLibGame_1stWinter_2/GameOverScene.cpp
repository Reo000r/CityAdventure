#include "GameOverScene.h"
#include "TitleScene.h"  // 遷移先のシーン
#include "GamePlayScene.h"  // 遷移先のシーン
#include "Scene.h"
#include "SceneController.h"
#include "CursorUI.h"

#include "Input.h"
#include "Game.h"

#include <memory>
#include <DxLib.h>
#include <cassert>

void GameOverScene::FadeinUpdate()
{
	_frame--;
	if (_frame <= 0)
	{
		_nowUpdateState = &GameOverScene::NormalUpdate;
		_nowDrawState = &GameOverScene::NormalDraw;
	}
}

void GameOverScene::NormalUpdate()
{
	_cursor->Update();

	// 決定を押して指定f経ったら
	if (_cursor->GetDecisionFrame() >= 60)
	{
		_nowUpdateState = &GameOverScene::FadeoutUpdate;
		_nowDrawState = &GameOverScene::FadeDraw;
		_frame = 0;
	}

	// 押されたらエフェクトを動かす
	// (雑)
	if (_cursor->GetDecisionFrame())
	{
		Vector2 vel = { 7.0f, 0.0f };
		_cursor->SetEffectVel(vel);
	}
}

void GameOverScene::FadeoutUpdate()
{
	_frame++;
	if (_frame >= Game::kFadeInterval)
	{
		// 0(上)だった場合
		if (!_cursor->GetCurrentPosition())
		{
			SceneController::GetInstance().ChangeScene(std::make_shared<GamePlayScene>());
			return;  // 自分が死んでいるのでもし
			// 余計な処理が入っているとまずいのでreturn;
		}
		else // 1(下)だった場合
		{
			SceneController::GetInstance().ChangeScene(std::make_shared<TitleScene>());
			return;  // 自分が死んでいるのでもし
			// 余計な処理が入っているとまずいのでreturn;
		}
	}
}

void GameOverScene::FadeDraw()
{
	// 背景の表示
	float a = 0.6f;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawGraph(0, -140, _backgroundGraphHandle, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	_cursor->Draw();

	DrawRotaGraph(Game::kScreenCenterWidth + 20, Game::kScreenCenterHeight - 100,
		1.5, 0.0f, _gameoverStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 100,
		0.5, 0.0f, _continueHighlightStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 200,
		0.5, 0.0f, _titleStringGraphHandle, true);

	// フェードイン/アウトの処理
	// フェード割合の計算(0.0-1.0)
	float rate = static_cast<float>(_frame) / static_cast<float>(Game::kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(255 * rate));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	// BlendModeを使った後はNOBLENDにしておくことを忘れず
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameOverScene::NormalDraw()
{
	float a = 0.6f;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawGraph(0, -140, _backgroundGraphHandle, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//// カーソル表示
	//DrawRotaGraph(_cursorPos.x, _cursorPos.y,
	//	1.5, 0.0f, _cursorGraphHandle, true);

	_cursor->Draw();

	DrawRotaGraph(Game::kScreenCenterWidth + 20, Game::kScreenCenterHeight - 100,
		1.5, 0.0f, _gameoverStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 100,
		0.5, 0.0f, _continueHighlightStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 200,
		0.5, 0.0f, _titleStringGraphHandle, true);

	//DrawFormatString(0, 0, 0xffffff, L"GameClear Scene");
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"GameOver Scene");
#endif
}

GameOverScene::GameOverScene() :
	_frame(Game::kFadeInterval),
	_backgroundGraphHandle(0),
	_gameoverStringGraphHandle(0),
	_cursor(std::make_shared<CursorUI>()),
	_nowUpdateState(&GameOverScene::FadeinUpdate),
	_nowDrawState(&GameOverScene::FadeDraw)
{
	_backgroundGraphHandle = LoadGraph(L"data/img/background/Background.png");
	assert(_backgroundGraphHandle >= 0);
	_gameoverStringGraphHandle = LoadGraph(L"data/img/string/GameOver.png");
	assert(_gameoverStringGraphHandle >= 0);
	_continueHighlightStringGraphHandle = LoadGraph(L"data/img/string/Continue_Highlight.png");
	assert(_continueHighlightStringGraphHandle >= 0);
	_titleStringGraphHandle = LoadGraph(L"data/img/string/Title.png");
	assert(_titleStringGraphHandle >= 0);
	_cursorUndecisionHandle = LoadGraph(L"data/img/player/gameover/GameOverPlayer_Idle.png");
	assert(_cursorUndecisionHandle >= 0);
	_cursorDecisionHandle = LoadGraph(L"data/img/player/gameover/GameOverPlayer_Run.png");
	assert(_cursorDecisionHandle >= 0);
	_cursorEffectHandle = LoadGraph(L"data/img/player/gameover/GameOverPlayer_Bullet.png");
	assert(_cursorEffectHandle >= 0);

	Vector2 pos    = GameoverSceneData::kStartCursorPos;
	Vector2 addPos = GameoverSceneData::kAddCursorPos;
	_cursor->Init(pos, addPos, 3.0f, 3.0f, _cursorUndecisionHandle, _cursorDecisionHandle, _cursorEffectHandle, true);
}

GameOverScene::~GameOverScene()
{
	DeleteGraph(_backgroundGraphHandle);
	DeleteGraph(_gameoverStringGraphHandle);
	DeleteGraph(_continueHighlightStringGraphHandle);
	DeleteGraph(_titleStringGraphHandle);
	DeleteGraph(_cursorUndecisionHandle);
	DeleteGraph(_cursorDecisionHandle);
	DeleteGraph(_cursorEffectHandle);
}

void GameOverScene::Update()
{
	(this->*_nowUpdateState)();
}

void GameOverScene::Draw()
{
	(this->*_nowDrawState)();
}
