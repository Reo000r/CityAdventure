#pragma once

class SceneController;

/// <summary>
/// �V�[���̊��N���X
/// ���͎̂����Ȃ�
/// </summary>
class Scene
{
public:
	Scene() {};

	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// �`��S��
	/// </summary>
	virtual void Draw() abstract;
};

