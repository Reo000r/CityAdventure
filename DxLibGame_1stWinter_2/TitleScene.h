#pragma once
#include "Scene.h"
#include "Vector2.h"

namespace
{
	constexpr int kBackgroundGraphOffsetX =    0;
	constexpr int kBackgroundGraphOffsetY = -140;

	constexpr int kTitleGraphOffsetX = -70;
	constexpr int kTitleGraphOffsetY =   0; // -90
}

class TitleScene : public Scene
{
private:
	int _frame;
	int _graphHandle;
	int _titleGraphHandle;
	int _backgroundGraphHandle;

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
};

