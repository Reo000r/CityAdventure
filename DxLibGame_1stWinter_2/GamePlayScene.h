#pragma once
#include "Scene.h"

#include <memory>

class Player;
class Map;
class GameSceneCamera;
class Physics;

class GamePlayScene : public Scene
{
private:
	// メンバ変数
	int _frame;
	int _playerIdleGraphHandle;
	int _playerRunGraphHandle;
	int _playerJumpGraphHandle;
	int _playerFallGraphHandle;
	int _mapGraphHandle;
	int _backgroundGraphHandle;

	std::shared_ptr<Player> _player;
	std::shared_ptr<Map> _map;
	std::shared_ptr<GameSceneCamera> _camera;
	//physicsの参照が欲しいな

	// UpdateとDrawのStateパターン
	// _updateや_drawが変数であることを分かりやすくしている
	using UpdateFunc_t = void(GamePlayScene::*)();
	using DrawFunc_t = void(GamePlayScene::*)();

	UpdateFunc_t _nowUpdateState = nullptr;
	DrawFunc_t _nowDrawState = nullptr;

	/// <summary>
	/// フェードイン時の更新処理
	/// </summary>
	void FadeinUpdate();
	// 通常時の更新処理
	void NormalUpdate();
	// フェードアウト時の更新処理
	void FadeoutUpdate();

	// フェード時の描画
	void FadeDraw();
	// 通常時の描画
	void NormalDraw();

public:
	GamePlayScene();
	~GamePlayScene();

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画全般
	/// </summary>
	virtual void Draw() override;
};

