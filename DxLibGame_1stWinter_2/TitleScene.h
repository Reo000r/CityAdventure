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
};

