#include "GamePlayScene.h"
#include "GameClearScene.h"  // �J�ڐ�̃V�[��
#include "GameOverScene.h"   // �J�ڐ�̃V�[��
#include "TitleScene.h"   // �J�ڐ�̃V�[��
#include "Scene.h"
#include "SceneController.h"
#include "Player.h"
#include "PlayerBulletController.h"
#include "EnemyController.h"
#include "EnemyBulletController.h"
#include "GoalObject.h"
#include "Map.h"
#include "GameSceneCamera.h"
#include "UIController.h"
#include "ScoreController.h"

#include "Input.h"
#include "Game.h"

#include <memory>
#include <DxLib.h>
#include <cassert>

void GamePlayScene::FadeinUpdate()
{
	_frame--;

	_camera->Update();

	if (_frame <= 0)
	{
		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetTimeCount(true);

		_nowUpdateState = &GamePlayScene::NormalUpdate;
		_nowDrawState = &GamePlayScene::NormalDraw;
	}
}

void GamePlayScene::NormalUpdate()
{
	_player->Update();
	_enemyController->Update();
	_goal->Update();
	_camera->Update();
	_uiController->Update();

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.Update();
	
	// �����������y���Ȃ�Ǝv���Ẵl�X�g
	if (!_goal->IsActive())
	{
		if (_enemyController->IsAllBossKill())
		{
			_goal->Active();
		}
	}
	
#ifdef _DEBUG
	//// �V�[���؂�ւ�(�f�o�b�O)
	//if (Input::GetInstance().IsTrigger(PAD_INPUT_4))
	//{
	//	ScoreController& scoreController = ScoreController::GetInstance();
	//	scoreController.SetTimeCount(false);

	//	_nowUpdateState = &GamePlayScene::FadeoutUpdate;
	//	_nowDrawState = &GamePlayScene::FadeDraw;
	//	_frame = 0;
	//}
#endif // _DEBUG

#ifdef RELOAD_GAME_STAGE
	// �V�[�������[�h
	if (Input::GetInstance().IsTrigger(INPUTRIGHTMENU))
	{
		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetTimeCount(false);
		_nextScene = std::make_shared<GamePlayScene>();

		_nowUpdateState = &GamePlayScene::FadeoutUpdate;
		_nowDrawState = &GamePlayScene::FadeDraw;
		_frame = 0;
	}
	// �^�C�g����
	if (Input::GetInstance().IsTrigger(INPUTLEFTMENU))
	{
		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetTimeCount(false);
		_nextScene = std::make_shared<TitleScene>();

		_nowUpdateState = &GamePlayScene::FadeoutUpdate;
		_nowDrawState = &GamePlayScene::FadeDraw;
		_frame = 0;
	}
#endif // RELOAD_GAME_STAGE

	// �v���C���[������ł�����Q�[���I�[�o�[�������s��
	if (_player->IsDead())
	{
		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetTimeCount(false);
		
		_nowUpdateState = &GamePlayScene::FadeoutUpdate;
		_nowDrawState = &GamePlayScene::FadeDraw;
		_frame = 0;
		_nextScene = std::make_shared<GameOverScene>();
	}

	// �v���C���[������ł�����Q�[���I�[�o�[�������s��
	if (ScoreController::GetInstance().IsTimeover())
	{
		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetTimeCount(false);
		
		_nowUpdateState = &GamePlayScene::FadeoutUpdate;
		_nowDrawState = &GamePlayScene::FadeDraw;
		_frame = 0;
		_nextScene = std::make_shared<GameOverScene>();
	}

	// �v���C���[���S�[��������S�[���������s��
	if (_player->IsGoal())
	{
		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.SetTimeCount(false);

		_nowUpdateState = &GamePlayScene::FadeoutUpdate;
		_nowDrawState = &GamePlayScene::FadeDraw;
		_frame = 0;
		_nextScene = std::make_shared<GameClearScene>();
	}
}

void GamePlayScene::FadeoutUpdate()
{
	_frame++;

	float volMul = static_cast<float>(GameplaySceneData::kFadeFrame - _frame) / GameplaySceneData::kFadeFrame;
	int titleVol = Game::kSoundVolume * GameplaySceneData::kBGMVolMul * volMul;
	if (titleVol >= 255) titleVol = 255;
	ChangeVolumeSoundMem(titleVol, _gameplayBGMHandle);

	_camera->Update();

	if (_frame >= GameplaySceneData::kFadeFrame)
	{
		StopSoundMem(_gameplayBGMHandle);

		ScoreController& scoreController = ScoreController::GetInstance();
		scoreController.ResultInit();

		SceneController::GetInstance().ChangeScene(_nextScene);
		return;  // ����������ł���̂ł���
		// �]�v�ȏ����������Ă���Ƃ܂����̂�return;
	}
}

void GamePlayScene::FadeDraw()
{
	DrawGraph(0, -140, _backgroundGraphHandle, false);

	_map->Draw();
	_player->Draw(_camera);
	_enemyController->Draw(_camera);
	_goal->Draw(_camera);
	_uiController->Draw(_camera);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.GameDraw();

	// �w�i�̕\��
	//DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);

	// �t�F�[�h�C��/�A�E�g�̏���
	// �t�F�[�h�����̌v�Z(0.0-1.0)
	float rate = static_cast<float>(_frame) / static_cast<float>(GameplaySceneData::kFadeFrame);
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(255 * rate));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	// BlendMode���g�������NOBLEND�ɂ��Ă������Ƃ�Y�ꂸ
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayScene::NormalDraw()
{
	DrawGraph(0, -140, _backgroundGraphHandle, false);

	_map->Draw();
	_player->Draw(_camera);
	_enemyController->Draw(_camera);
	_goal->Draw(_camera);
	_uiController->Draw(_camera);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.GameDraw();

	//DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);
	

	// �X�R�A��\������\��̏�
#ifdef _DEBUG
	//DrawFormatString(0, 0, 0xffffff, L"GamePlay Scene");

	//Vector2 strPos = { Game::kScreenWidth - 10 -32, 10 };
	//int addX = 0;
	//for (int i = 0; i < 6; i++)
	//{
	//	DrawBox(static_cast<int>(strPos.x + addX),
	//		static_cast<int>(strPos.y),
	//		static_cast<int>(strPos.x + 24 + addX),
	//		static_cast<int>(strPos.y + 32),
	//		0xffffff, true);
	//	addX -= 24 + 8;
	//}
#endif // _DEBUG
}

GamePlayScene::GamePlayScene() :
	_frame(GameplaySceneData::kFadeFrame),
	_playerIdleGraphHandle(0),
	_playerRunGraphHandle(0),
	_playerJumpGraphHandle(0),
	_playerFallGraphHandle(0),
	_nextScene(std::make_shared<GameClearScene>()),
	_nowUpdateState(&GamePlayScene::FadeinUpdate),
	_nowDrawState(&GamePlayScene::FadeDraw),
	_player(std::make_shared<Player>()),
	_playerBulletController(std::make_shared<PlayerBulletController>()),
	_enemyController(std::make_shared<EnemyController>()),
	_enemyBulletController(std::make_shared<EnemyBulletController>()),
	_goal(std::make_shared<GoalObject>()),
	_map(std::make_shared<Map>()),
	_camera(std::make_shared<GameSceneCamera>(*_player)),
	_uiController(std::make_shared<UIController>())
{
	_playerIdleGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Idle.png");
	assert(_playerIdleGraphHandle >= 0);
	_playerRunGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Run.png");
	assert(_playerRunGraphHandle >= 0);
	_playerJumpGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Jump.png");
	assert(_playerJumpGraphHandle >= 0);
	_playerFallGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Fall.png");
	assert(_playerFallGraphHandle >= 0);
	_playerDeathGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Death.png");
	assert(_playerDeathGraphHandle >= 0);
	_goalGraphHandle = LoadGraph(L"data/img/goal/Goal.png");
	assert(_goalGraphHandle >= 0);
	_mapGraphHandle = LoadGraph(L"data/img/map/Map.png");
	assert(_mapGraphHandle >= 0);

	_backgroundGraphHandle = LoadGraph(L"data/img/background/Background.png");
	assert(_backgroundGraphHandle >= 0);
	// bgm
	_gameplayBGMHandle = LoadSoundMem(L"data/sound/bgm/GameSceneBGM.mp3");
	assert(_gameplayBGMHandle >= 0);

	// �v���C���[�Ɏ������g��ptr��n������
	//std::weak_ptr<GamePlayScene> ptr = std::weak_ptr<GamePlayScene>();

	_player->Init(
		_goal,
		_map,
		_playerBulletController,
		_playerIdleGraphHandle,
		_playerRunGraphHandle,
		_playerJumpGraphHandle,
		_playerFallGraphHandle,
		_playerDeathGraphHandle);

	_enemyController->Init(_map, _player, _playerBulletController, _enemyBulletController);

	_goal->Init(_goalGraphHandle);

	_map->Init(_mapGraphHandle, _camera);

	_uiController->Init(_player);

	ScoreController& scoreController = ScoreController::GetInstance();
	scoreController.Init();

	
}

GamePlayScene::~GamePlayScene()
{
	DeleteGraph(_playerIdleGraphHandle);
	DeleteGraph(_playerRunGraphHandle);
	DeleteGraph(_playerJumpGraphHandle);
	DeleteGraph(_playerFallGraphHandle);
	DeleteGraph(_playerDeathGraphHandle);
	DeleteGraph(_goalGraphHandle);
	DeleteGraph(_mapGraphHandle);
	DeleteGraph(_backgroundGraphHandle);

	DeleteSoundMem(_gameplayBGMHandle);
}

void GamePlayScene::Update()
{
	(this->*_nowUpdateState)();
}

void GamePlayScene::Draw()
{
	(this->*_nowDrawState)();
}

void GamePlayScene::StartBGM()
{
	// bgm�Đ�
	PlaySoundMem(_gameplayBGMHandle, DX_PLAYTYPE_LOOP);
	int titleVol = Game::kSoundVolume * GameplaySceneData::kBGMVolMul;
	if (titleVol >= 255) titleVol = 255;
	ChangeVolumeSoundMem(titleVol, _gameplayBGMHandle);
}
