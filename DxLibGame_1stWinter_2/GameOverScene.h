#pragma once
#include "Scene.h"
#include "Vector2.h"

#include <memory>

class CursorUI;

namespace GameoverSceneData
{
	// �J�[�\���̋N�_�ʒu
	// �����\���̊�ł�����
	constexpr int kStartCursorPosX = 400;
	constexpr int kStartCursorPosY = 460;

	const Vector2 kStartCursorPos = { kStartCursorPosX, kStartCursorPosY };

	constexpr int kAddCursorPosX = 0;
	constexpr int kAddCursorPosY = 100;

	const Vector2 kAddCursorPos = { kAddCursorPosX, kAddCursorPosY };

	// �����\���̕␳�l

}

class GameOverScene : public Scene
{
private:
	int _frame;
	int _backgroundGraphHandle;
	int _gameoverStringGraphHandle;
	int _continueHighlightStringGraphHandle;
	int _titleStringGraphHandle;
	int _cursorUndecisionHandle;
	int _cursorDecisionHandle;
	int _cursorEffectHandle;

	std::shared_ptr<CursorUI> _cursor;

	// _update��_draw���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void(GameOverScene::*)();
	using DrawFunc_t = void(GameOverScene::*)();

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
	GameOverScene();
	~GameOverScene();

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// �`��S��
	/// </summary>
	virtual void Draw() override;
};

