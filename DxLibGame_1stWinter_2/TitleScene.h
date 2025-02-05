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

	// �J�[�\���̋N�_�ʒu
	// �����\���̊�ł�����
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

	// _update��_draw���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void (TitleScene::*)();
	using DrawFunc_t = void (TitleScene::*)();

	UpdateFunc_t _nowUpdateState = nullptr;
	DrawFunc_t _nowDrawState = nullptr;

	// �t�F�[�h�C�����̍X�V����
	void FadeinUpdate();
	// �ʏ펞�̍X�V����
	void NormalUpdate();
	// �t�F�[�h�A�E�g���̍X�V����
	void FadeoutUpdate();

	// �t�F�[�h���̕`��
	void FadeDraw();
	// �ʏ펞�̕`��
	void NormalDraw();

public:
	TitleScene();
	~TitleScene();

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// �`��S��
	/// </summary>
	virtual void Draw() override;

	void StartBGM() override;
};

