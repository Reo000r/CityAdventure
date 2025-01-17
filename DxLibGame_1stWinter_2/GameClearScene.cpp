#include "GameClearScene.h"
#include "TitleScene.h"  // �J�ڐ�̃V�[��
#include "Scene.h"
#include "SceneController.h"

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
	if (Input::GetInstance().IsTrigger(PAD_INPUT_1))
	{
		_nowUpdateState = &GameClearScene::FadeoutUpdate;
		_nowDrawState = &GameClearScene::FadeDraw;
		_frame = 0;
	}
}

void GameClearScene::FadeoutUpdate()
{
	_frame++;
	if (_frame >= Game::kFadeInterval)
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<TitleScene>());
		return;  // ����������ł���̂ł���
		// �]�v�ȏ����������Ă���Ƃ܂����̂�return;
	}
}

void GameClearScene::FadeDraw()
{
	// �w�i�̕\��
	DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);

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
	DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);
	DrawFormatString(0, 0, 0xffffff, L"GameClear Scene");
}

GameClearScene::GameClearScene() :
	_frame(Game::kFadeInterval),
	_graphHandle(0),
	_nowUpdateState(&GameClearScene::FadeinUpdate),
	_nowDrawState(&GameClearScene::FadeDraw)
{
	_graphHandle = LoadGraph(L"data/img/player/2/Player2_Idle.png");
	assert(_graphHandle >= 0);
}

GameClearScene::~GameClearScene()
{
	DeleteGraph(_graphHandle);
}

void GameClearScene::Update()
{
	(this->*_nowUpdateState)();
}

void GameClearScene::Draw()
{
	(this->*_nowDrawState)();
}
