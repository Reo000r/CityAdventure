#include "TitleScene.h"
#include "GamePlayScene.h"  // 遷移先のシーン
#include "Scene.h"
#include "SceneController.h"
#include "CursorUI.h"

#include "Input.h"
#include "Game.h"

#include <memory>
#include <DxLib.h>
#include <cassert>

void TitleScene::FadeinUpdate()
{
	_frame--;
	_titleFadeFrameCount++;
	if (_frame <= 0)
	{
		_nowUpdateState = &TitleScene::NormalUpdate;
		_nowDrawState = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate()
{
	_titleFadeFrameCount++;

	_cursor->Update();

	// 終わっていたら
	if (_titleFadeFrameCount >= TitleSceneData::kTitleFadeFrame + 120)
	{
		_startTitleAnim = false;
		// 入力を受け付ける
		_cursor->SetCanInput(true);
		_cursor->SetUseHandle(_cursorUndecisionHandle);
		_cursor->SetCursorVel({});
	}
	// アニメーションが始まっていない かつ 1/2終わっていたら
	else if(!_startTitleAnim &&
			_titleFadeFrameCount >= TitleSceneData::kTitleFadeFrame / 2)
	{
		_startTitleAnim = true;
		_cursor->SetUseHandle(_cursorDecisionHandle);
		Vector2 vel = {5.0f, 0.0f};
		_cursor->SetCursorVel(vel);
	}

	// 押されたらエフェクトを動かす
	if (_cursor->GetDecisionFrame())
	{
		_cursor->SetUseHandle(_cursorDecisionHandle);
		Vector2 vel = { 8.0f, 0.0f };
		_cursor->SetEffectVel(vel);
	}

	// 決定を押して一定時間経ったら
	if (_cursor->GetDecisionFrame() >= TitleSceneData::kFadeFrame)
	{
		_nowUpdateState = &TitleScene::FadeoutUpdate;
		_nowDrawState = &TitleScene::FadeDraw;
		_frame = 0;
	}
}

void TitleScene::FadeoutUpdate()
{
	_frame++;
	float volMul = static_cast<float>(TitleSceneData::kFadeFrame - _frame) / TitleSceneData::kFadeFrame;
	int titleVol = Game::kSoundVolume * TitleSceneData::kBGMVolMul * volMul;
	if (titleVol >= 255) titleVol = 255;
	ChangeVolumeSoundMem(titleVol, _titleBGMHandle);

	if (_frame >= TitleSceneData::kFadeFrame)
	{
		StopSoundMem(_titleBGMHandle);
		SceneController::GetInstance().ChangeScene(std::make_shared<GamePlayScene>());
		return;  // 自分が死んでいるのでもし
		// 余計な処理が入っているとまずいのでreturn;
	}
}

void TitleScene::FadeDraw()
{
	// 背景の表示
	DrawGraph(TitleSceneData::kBackgroundGraphOffsetX, TitleSceneData::kBackgroundGraphOffsetY, _backgroundGraphHandle, false);
	
	// タイトル
	// 0が透明
	float a = static_cast<float>(_titleFadeFrameCount) / TitleSceneData::kTitleFadeFrame;
	if (a > 1.0f) a = 1.0f;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawGraph(TitleSceneData::kTitleGraphOffsetX, TitleSceneData::kTitleGraphOffsetY, _titleGraphHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	_cursor->Draw();

	// start
	a = static_cast<float>(_titleFadeFrameCount - TitleSceneData::kTitleFadeFrame / 2) / TitleSceneData::kTitleFadeFrame;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 200,
		0.5, 0.0f, _startHighlightStringGraphHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
	// 背景の表示
	DrawGraph(TitleSceneData::kBackgroundGraphOffsetX, TitleSceneData::kBackgroundGraphOffsetY, _backgroundGraphHandle, false);
	
	// タイトル
	// 0が透明
	float a = static_cast<float>(_titleFadeFrameCount) / TitleSceneData::kTitleFadeFrame;
	if (a > 1.0f) a = 1.0f;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawGraph(TitleSceneData::kTitleGraphOffsetX, TitleSceneData::kTitleGraphOffsetY, _titleGraphHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	_cursor->Draw();

	// start
	a = static_cast<float>(_titleFadeFrameCount - TitleSceneData::kTitleFadeFrame/2) / TitleSceneData::kTitleFadeFrame;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 200,
		0.5, 0.0f, _startHighlightStringGraphHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"Title Scene");
#endif
}

TitleScene::TitleScene() :
	_frame(Game::kFadeInterval),
	_titleFadeFrameCount(0),
	_startTitleAnim(false),
	_cursor(std::make_shared<CursorUI>()),
	_nowUpdateState(&TitleScene::FadeinUpdate),
	_nowDrawState(&TitleScene::FadeDraw)
{
	_titleGraphHandle = LoadGraph(L"data/img/background/Title.png");
	assert(_titleGraphHandle >= 0);
	_backgroundGraphHandle = LoadGraph(L"data/img/background/Background.png");
	assert(_backgroundGraphHandle >= 0);
	// string
	_startHighlightStringGraphHandle = LoadGraph(L"data/img/string/Start_Highlight.png");
	assert(_startHighlightStringGraphHandle >= 0);
	// cursor
	_cursorUndecisionHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Idle.png");
	assert(_cursorUndecisionHandle >= 0);
	_cursorDecisionHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Run.png");
	assert(_cursorDecisionHandle >= 0);
	_cursorEffectHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Bullet.png");
	assert(_cursorEffectHandle >= 0);
	// bgm
	_titleBGMHandle = LoadSoundMem(L"data/sound/bgm/TitleSceneBGM.mp3");
	assert(_titleBGMHandle >= 0);

	Vector2 pos    = TitleSceneData::kStartCursorPos;
	Vector2 addPos = TitleSceneData::kAddCursorPos;
	// 入力はfalse
	_cursor->Init(pos, addPos, 5.0f, 4.0f, _cursorUndecisionHandle, _cursorDecisionHandle, _cursorEffectHandle, false);

	_cursor->SetSingleAnimFrame(8, 8, 5);
}

TitleScene::~TitleScene()
{
	DeleteGraph(_titleGraphHandle);
	DeleteGraph(_backgroundGraphHandle);
	DeleteGraph(_startHighlightStringGraphHandle);
	DeleteGraph(_cursorUndecisionHandle);
	DeleteGraph(_cursorDecisionHandle);
	DeleteGraph(_cursorEffectHandle);

	DeleteSoundMem(_titleBGMHandle);
}

void TitleScene::Update()
{
	(this->*_nowUpdateState)();
}

void TitleScene::Draw()
{
	(this->*_nowDrawState)();
}

void TitleScene::StartBGM()
{
	// bgm再生
	PlaySoundMem(_titleBGMHandle, DX_PLAYTYPE_LOOP);
	int titleVol = Game::kSoundVolume * TitleSceneData::kBGMVolMul;
	if (titleVol >= 255) titleVol = 255;
	ChangeVolumeSoundMem(titleVol, _titleBGMHandle);
}
