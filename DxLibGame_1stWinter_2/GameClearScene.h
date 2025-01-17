#pragma once
#include "Scene.h"

class GameClearScene : public Scene
{
private:
	int _frame;
	int _graphHandle;

	// _update��_draw���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void(GameClearScene::*)();
	using DrawFunc_t = void(GameClearScene::*)();

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
	GameClearScene();
	~GameClearScene();

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// �`��S��
	/// </summary>
	virtual void Draw() override;
};

