#pragma once
#include "Scene.h"

class GameClearScene : public Scene
{
private:
	int _frame;
	int _graphHandle;

	// _updateや_drawが変数であることを分かりやすくしている
	using UpdateFunc_t = void(GameClearScene::*)();
	using DrawFunc_t = void(GameClearScene::*)();

	UpdateFunc_t _nowUpdateState = nullptr;
	DrawFunc_t _nowDrawState = nullptr;

	// フェードイン時の更新処理
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
	GameClearScene();
	~GameClearScene();

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画全般
	/// </summary>
	virtual void Draw() override;
};

