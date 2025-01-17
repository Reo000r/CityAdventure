#include "GamePlayScene.h"
#include "GameClearScene.h"  // 遷移先のシーン
#include "Scene.h"
#include "SceneController.h"
#include "Player.h"
#include "Map.h"
#include "GameSceneCamera.h"
//#include "Physics.h"

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
		_nowUpdateState = &GamePlayScene::NormalUpdate;
		_nowDrawState = &GamePlayScene::NormalDraw;
	}
}

void GamePlayScene::NormalUpdate()
{
	_player->Update();
	_camera->Update();
	//Physics::GetInstance().Update();

	if (Input::GetInstance().IsTrigger(PAD_INPUT_1))
	{
		_nowUpdateState = &GamePlayScene::FadeoutUpdate;
		_nowDrawState = &GamePlayScene::FadeDraw;
		_frame = 0;
	}
}

void GamePlayScene::FadeoutUpdate()
{
	_frame++;

	_camera->Update();

	if (_frame >= Game::kFadeInterval)
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<GameClearScene>());
		return;  // 自分が死んでいるのでもし
		// 余計な処理が入っているとまずいのでreturn;
	}
}

void GamePlayScene::FadeDraw()
{
	DrawGraph(0, 0, _backgroundGraphHandle, false);

	_map->Draw(*_camera);
	_player->Draw(*_camera);

	// 背景の表示
	//DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);

	// フェードイン/アウトの処理
	// フェード割合の計算(0.0-1.0)
	float rate = static_cast<float>(_frame) / static_cast<float>(Game::kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * rate);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	// BlendModeを使った後はNOBLENDにしておくことを忘れず
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayScene::NormalDraw()
{
	DrawGraph(0, -140, _backgroundGraphHandle, false);

	_map->Draw(*_camera);
	_player->Draw(*_camera);

	//DrawRotaGraph(static_cast<int>(Game::kScreenCenterWidth), static_cast<int>(Game::kScreenCenterHeight), 1.0, 0.0f, _graphHandle, true);
	DrawFormatString(0, 0, 0xffffff, L"GamePlay Scene");

	// スコアを表示する予定の所
#ifdef _DEBUG
	Vector2 strPos = { Game::kScreenWidth - 10 -32, 10 };
	int addX = 0;
	for (int i = 0; i < 6; i++)
	{
		DrawBox(strPos.x + addX, strPos.y, strPos.x + 24 + addX, strPos.y + 32, 0xffffff, true);
		addX -= 24 + 8;
	}
#endif // _DEBUG
}

GamePlayScene::GamePlayScene() :
	_frame(Game::kFadeInterval),
	_playerIdleGraphHandle(0),
	_playerRunGraphHandle(0),
	_playerJumpGraphHandle(0),
	_playerFallGraphHandle(0),
	_nowUpdateState(&GamePlayScene::FadeinUpdate),
	_nowDrawState(&GamePlayScene::FadeDraw),
	_player(std::make_shared<Player>()),
	_map(std::make_shared<Map>()),
	_camera(std::make_shared<GameSceneCamera>(*_player))
{
	_playerIdleGraphHandle = LoadGraph(L"data/img/player/0/Player0_Idle.png");
	assert(_playerIdleGraphHandle >= 0);
	_playerRunGraphHandle = LoadGraph(L"data/img/player/0/Player0_Run.png");
	assert(_playerRunGraphHandle >= 0);
	_playerJumpGraphHandle = LoadGraph(L"data/img/player/0/Player0_Jump.png");
	assert(_playerJumpGraphHandle >= 0);
	_playerFallGraphHandle = LoadGraph(L"data/img/player/0/Player0_Fall.png");
	assert(_playerFallGraphHandle >= 0);
	_mapGraphHandle = LoadGraph(L"data/img/map/Map.png");
	assert(_mapGraphHandle >= 0);

	_backgroundGraphHandle = LoadGraph(L"data/img/background/Background.png");
	assert(_backgroundGraphHandle >= 0);

	_player->Init(
		_map,
		_playerIdleGraphHandle,
		_playerRunGraphHandle,
		_playerJumpGraphHandle,
		_playerFallGraphHandle);

	_map->Init(_mapGraphHandle, _camera);
}

GamePlayScene::~GamePlayScene()
{
	DeleteGraph(_playerIdleGraphHandle);
	DeleteGraph(_playerRunGraphHandle);
	DeleteGraph(_playerJumpGraphHandle);
	DeleteGraph(_playerFallGraphHandle);
	DeleteGraph(_mapGraphHandle);
	DeleteGraph(_backgroundGraphHandle);
}

void GamePlayScene::Update()
{
	(this->*_nowUpdateState)();
}

void GamePlayScene::Draw()
{
	(this->*_nowDrawState)();
}