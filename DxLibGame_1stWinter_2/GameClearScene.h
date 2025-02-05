#pragma once
#include "Scene.h"
#include "Vector2.h"

#include <memory>

class CursorUI;

namespace GameclearSceneData
{
	// カーソルの起点位置
	// 文字表示の基準でもある
	constexpr int kStartCursorPosX = 400;
	constexpr int kStartCursorPosY = 460;

	const Vector2 kStartCursorPos = { kStartCursorPosX, kStartCursorPosY };

	constexpr int kAddCursorPosX = 0;
	constexpr int kAddCursorPosY = 100;

	const Vector2 kAddCursorPos = { kAddCursorPosX, kAddCursorPosY };

	// 文字表示の補正値

}

class GameClearScene : public Scene
{
private:
	int _frame;
	int _backgroundGraphHandle;
	int _clearStringGraphHandle;
	int _continueHighlightStringGraphHandle;
	int _titleStringGraphHandle;
	int _cursorUndecisionHandle;
	int _cursorDecisionHandle;
	int _cursorEffectHandle;

	std::shared_ptr<CursorUI> _cursor;

	// _updateや_drawが変数であることを分かりやすくしている
	using UpdateFunc_t = void(GameClearScene::*)();
	using DrawFunc_t = void(GameClearScene::*)();

	UpdateFunc_t _nowUpdateState = nullptr;
	DrawFunc_t   _nowDrawState   = nullptr;

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

