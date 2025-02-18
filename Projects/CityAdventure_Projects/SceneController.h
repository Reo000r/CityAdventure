#pragma once
#include <memory>
#include <list>

class Scene;

/// <summary>
/// �e�V�[�����Ǘ�����
/// �V���O���g��������
/// </summary>
class SceneController
{
private:

	// �V���O���g���ɂ���
	SceneController();
	SceneController(const SceneController&) = delete;
	void operator=(const SceneController&) = delete;

	/// <summary>
	/// ���s���̃V�[���̃|�C���^
	/// </summary>
	using SceneStace_t = std::list<std::shared_ptr<Scene>>;
	SceneStace_t _scenes;

public:
	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>SceneController�V���O���g���I�u�W�F�N�g</returns>
	static SceneController& GetInstance();

	/// <summary>
	/// Application����Ăяo�����Update
	/// ������Scene��Update���Ăяo��
	/// </summary>
	void Update();

	/// <summary>
	/// Application����Ăяo�����Draw
	/// ������Scene��Draw���Ăяo��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// </summary>
	/// <param name="scene">���̏��</param>
	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���ݎ��s���̃V�[���̏�ɕʂ̃V�[�����悹��
	/// </summary>
	/// <param name="scene">�悹�����V�[��</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���ݕ\��(�ŏ㕔)�ɂ�����s���̃V�[������菜��
	/// </summary>
	void PopScene();
};

