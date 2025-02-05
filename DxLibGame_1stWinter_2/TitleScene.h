#pragma once
#include "Scene.h"
#include "Vector2.h"
#include "Game.h"

#include <memory>

class CursorUI;

namespace TitleSceneData
{
	constexpr int kBackgroundGraphOffsetX =    0;
	constexpr int kBackgroundGraphOffsetY = -140;

	constexpr int kTitleGraphOffsetX = -70;
	constexpr int kTitleGraphOffsetY = -90; // -90

	// 
	constexpr int kTitleFadeFrame = static_cast<int>(Game::kFadeInterval * 1.5);

	// カーソルの起点位置
	// 文字表示の基準でもある
	constexpr int kStartCursorPosX = -256;
	constexpr int kStartCursorPosY = 570;

	const Vector2 kStartCursorPos = { kStartCursorPosX, kStartCursorPosY };

	constexpr int kAddCursorPosX = 0;
	constexpr int kAddCursorPosY = 0;

	const Vector2 kAddCursorPos = { kAddCursorPosX, kAddCursorPosY };

	constexpr int kFadeFrame = 60;
	constexpr float kBGMVolMul = 1.0f;
}

class TitleScene : public Scene
{
private:
	int _frame;
	int _titleFadeFrameCount;
	int _titleGraphHandle;
	int _titleBGMHandle;
	int _backgroundGraphHandle;
	int _startHighlightStringGraphHandle;

	int _cursorUndecisionHandle;
	int _cursorDecisionHandle;
	int _cursorEffectHandle;

	bool _startTitleAnim;

	std::shared_ptr<CursorUI> _cursor;

	// _updateや_drawが変数であることを分かりやすくしている
	using UpdateFunc_t = void (TitleScene::*)();
	using DrawFunc_t = void (TitleScene::*)();

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
	TitleScene();
	~TitleScene();

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画全般
	/// </summary>
	virtual void Draw() override;

	void StartBGM() override;
};

