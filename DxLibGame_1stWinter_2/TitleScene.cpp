#include "TitleScene.h"
#include "GamePlayScene.h"  // 遷移先のシーン
#include "Scene.h"
#include "SceneController.h"

#include "Input.h"
#include "Game.h"

#include <memory>
#include <DxLib.h>
#include <cassert>

void TitleScene::FadeinUpdate()
{
	_frame--;
	if (_frame <= 0)
	{
		_nowUpdateState = &TitleScene::NormalUpdate;
		_nowDrawState = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate()
{
	//if (Input::GetInstance().IsTrigger(KEY_INPUT_RETURN))
	if (Input::GetInstance().IsTrigger(PAD_INPUT_1))
	{
		_nowUpdateState = &TitleScene::FadeoutUpdate;
		_nowDrawState = &TitleScene::FadeDraw;
		_frame = 0;
	}
}

void TitleScene::FadeoutUpdate()
{
	_frame++;
	if (_frame >= 60)
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<GamePlayScene>());
		return;  // 自分が死んでいるのでもし
		// 余計な処理が入っているとまずいのでreturn;
	}
}

void TitleScene::FadeDraw()
{
	DrawGraph(kBackgroundGraphOffsetX, kBackgroundGraphOffsetY, _backgroundGraphHandle, false);
	DrawGraph(kTitleGraphOffsetX, kTitleGraphOffsetY, _titleGraphHandle, true);

	// 背景の表示
	DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);

	// フェードイン/アウトの処理
	// フェード割合の計算(0.0-1.0)
	float rate = static_cast<float>(_frame) / static_cast<float>(Game::kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(255 * rate));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	// BlendModeを使った後はNOBLENDにしておくことを忘れず
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawGraph(kBackgroundGraphOffsetX, kBackgroundGraphOffsetY, _backgroundGraphHandle, false);
	DrawGraph(kTitleGraphOffsetX, kTitleGraphOffsetY, _titleGraphHandle, true);

	DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);
	DrawFormatString(0, 0, 0xffffff, L"Title Scene");
}

TitleScene::TitleScene() :
	_frame(Game::kFadeInterval),
	_nowUpdateState(&TitleScene::FadeinUpdate),
	_nowDrawState(&TitleScene::FadeDraw)
{
	_graphHandle = LoadGraph(L"data/img/player/0/Player0_Idle.png");
	assert(_graphHandle >= 0);

	_titleGraphHandle = LoadGraph(L"data/img/background/Title.png");
	assert(_titleGraphHandle >= 0);
	_backgroundGraphHandle = LoadGraph(L"data/img/background/Background.png");
	assert(_backgroundGraphHandle >= 0);
}

TitleScene::~TitleScene()
{
	DeleteGraph(_graphHandle);
}

void TitleScene::Update()
{
	(this->*_nowUpdateState)();
}

void TitleScene::Draw()
{
	(this->*_nowDrawState)();
}
