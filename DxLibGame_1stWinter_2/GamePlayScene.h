#pragma once
#include "Scene.h"

#include <memory>
#include <vector>

class Player;
class EnemyController;
class EnemyBulletController;
class GoalObject;
class Map;
class GameSceneCamera;
class UIController;
class PlayerBulletController;

namespace GameplaySceneData
{
	constexpr int kFadeFrame = 60;
	constexpr float kBGMVolMul = 1.0f;
}

class GamePlayScene : public Scene
{
private:
	// �����o�ϐ�
	int _frame;
	int _playerIdleGraphHandle;
	int _playerRunGraphHandle;
	int _playerJumpGraphHandle;
	int _playerFallGraphHandle;
	int _playerDeathGraphHandle;
	int _goalGraphHandle;
	int _mapGraphHandle;
	int _backgroundGraphHandle;
	int _gameplayBGMHandle;

	// ���̃V�[��
	// �ÖٓI�ɕϊ����Ă������ۂ�
	std::shared_ptr<Scene> _nextScene;

	std::shared_ptr<Player> _player;
	std::shared_ptr<PlayerBulletController> _playerBulletController;
	std::shared_ptr<EnemyController> _enemyController;
	std::shared_ptr<EnemyBulletController> _enemyBulletController;
	std::shared_ptr<GoalObject> _goal; // map�Ƃ܂Ƃ߂�������
	std::shared_ptr<Map> _map;
	std::shared_ptr<GameSceneCamera> _camera;
	std::shared_ptr<UIController> _uiController;
	//physics�̎Q�Ƃ��~������

	// Update��Draw��State�p�^�[��
	// _update��_draw���ϐ��ł��邱�Ƃ𕪂���₷�����Ă���
	using UpdateFunc_t = void(GamePlayScene::*)();
	using DrawFunc_t = void(GamePlayScene::*)();

	UpdateFunc_t _nowUpdateState = nullptr;
	DrawFunc_t _nowDrawState = nullptr;

	/// <summary>
	/// �t�F�[�h�C�����̍X�V����
	/// </summary>
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
	GamePlayScene();
	~GamePlayScene();

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

