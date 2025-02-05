#include "GameClearScene.h"
#include "TitleScene.h"  // �J�ڐ�̃V�[��
#include "GamePlayScene.h"  // �J�ڐ�̃V�[��
#include "Scene.h"
#include "SceneController.h"
#include "CursorUI.h"

#include "Input.h"
#include "Game.h"

#include <memory>
#include <DxLib.h>
#include <cassert>

void GameClearScene::FadeinUpdate()
{
	_frame--;
	if (_frame <= 0)
	{
		_nowUpdateState = &GameClearScene::NormalUpdate;
		_nowDrawState = &GameClearScene::NormalDraw;
	}
}

void GameClearScene::NormalUpdate()
{
	_cursor->Update();

	if (_cursor->GetDecisionFrame() >= 60)
	{
		_nowUpdateState = &GameClearScene::FadeoutUpdate;
		_nowDrawState = &GameClearScene::FadeDraw;
		_frame = 0;
	}

	// �����ꂽ��G�t�F�N�g�𓮂���
	// (�G)
	if (_cursor->GetDecisionFrame())
	{
		Vector2 vel = { 7.0f, 0.0f };
		_cursor->SetEffectVel(vel);
	}
}

void GameClearScene::FadeoutUpdate()
{
	_frame++;
	if (_frame >= Game::kFadeInterval)
	{
		// 0(��)�������ꍇ
		if (!_cursor->GetCurrentPosition())
		{
			SceneController::GetInstance().ChangeScene(std::make_shared<GamePlayScene>());
			return;  // ����������ł���̂ł���
			// �]�v�ȏ����������Ă���Ƃ܂����̂�return;
		}
		else // 1(��)�������ꍇ
		{
			SceneController::GetInstance().ChangeScene(std::make_shared<TitleScene>());
			return;  // ����������ł���̂ł���
			// �]�v�ȏ����������Ă���Ƃ܂����̂�return;
		}
	}
}

void GameClearScene::FadeDraw()
{
	// �w�i�̕\��
	//float a = 0.6f;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawGraph(0, -140, _backgroundGraphHandle, false);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	_cursor->Draw();

	// ������\��
	DrawRotaGraph(Game::kScreenCenterWidth + 20, Game::kScreenCenterHeight - 100,
		1.5, 0.0f, _clearStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 100,
		0.5, 0.0f, _continueHighlightStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 200,
		0.5, 0.0f, _titleStringGraphHandle, true);

	// �t�F�[�h�C��/�A�E�g�̏���
	// �t�F�[�h�����̌v�Z(0.0-1.0)
	float rate = static_cast<float>(_frame) / static_cast<float>(Game::kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(255 * rate));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	// BlendMode���g�������NOBLEND�ɂ��Ă������Ƃ�Y�ꂸ
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameClearScene::NormalDraw()
{
	//float a = 0.6f;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * a));
	DrawGraph(0, -140, _backgroundGraphHandle, false);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	_cursor->Draw();
	
	// ������\��
	DrawRotaGraph(Game::kScreenCenterWidth + 20, Game::kScreenCenterHeight - 100, 
		1.5, 0.0f, _clearStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 100, 
		0.5, 0.0f, _continueHighlightStringGraphHandle, true);
	DrawRotaGraph(Game::kScreenCenterWidth, Game::kScreenCenterHeight + 200, 
		0.5, 0.0f, _titleStringGraphHandle, true);

	//DrawFormatString(0, 0, 0xffffff, L"GameClear Scene");
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"GameClear Scene");
#endif
}

GameClearScene::GameClearScene() :
	_frame(Game::kFadeInterval),
	_backgroundGraphHandle(0),
	_clearStringGraphHandle(0),
	_cursor(std::make_shared<CursorUI>()),
	_nowUpdateState(&GameClearScene::FadeinUpdate),
	_nowDrawState(&GameClearScene::FadeDraw)
{
	_backgroundGraphHandle = LoadGraph(L"data/img/background/Background.png");
	assert(_backgroundGraphHandle >= 0);
	_clearStringGraphHandle = LoadGraph(L"data/img/string/GameClear.png");
	assert(_clearStringGraphHandle >= 0);
	_continueHighlightStringGraphHandle = LoadGraph(L"data/img/string/Continue_Highlight.png");
	assert(_continueHighlightStringGraphHandle >= 0);
	_titleStringGraphHandle = LoadGraph(L"data/img/string/Title.png");
	assert(_titleStringGraphHandle >= 0);
	_cursorUndecisionHandle = LoadGraph(L"data/img/player/clear/ClearPlayer_Idle.png");
	assert(_cursorUndecisionHandle >= 0);
	_cursorDecisionHandle = LoadGraph(L"data/img/player/clear/ClearPlayer_Run.png");
	assert(_cursorDecisionHandle >= 0);
	_cursorEffectHandle = LoadGraph(L"data/img/player/clear/ClearPlayer_Bullet.png");
	assert(_cursorEffectHandle >= 0);

	Vector2 pos    = GameclearSceneData::kStartCursorPos;
	Vector2 addPos = GameclearSceneData::kAddCursorPos;
	_cursor->Init(pos, addPos, 3.0f, 3.0f, _cursorUndecisionHandle, _cursorDecisionHandle, _cursorEffectHandle, true);
}

GameClearScene::~GameClearScene()
{
	DeleteGraph(_backgroundGraphHandle);
	DeleteGraph(_clearStringGraphHandle);
	DeleteGraph(_continueHighlightStringGraphHandle);
	DeleteGraph(_titleStringGraphHandle);
	DeleteGraph(_cursorUndecisionHandle);
	DeleteGraph(_cursorDecisionHandle);
	DeleteGraph(_cursorEffectHandle);
}

void GameClearScene::Update()
{
	(this->*_nowUpdateState)();
}

void GameClearScene::Draw()
{
	(this->*_nowDrawState)();
}
