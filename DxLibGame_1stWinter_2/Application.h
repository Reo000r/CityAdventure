#pragma once

class Application
{
private:
	Application() {}
	// �Ȃ����ʂ��Ă��܂��̂�=delete��t����
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	int _frame = 0;

public:
	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>Application�V���O���g���I�u�W�F�N�g</returns>
	static Application& GetInstance();

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>true:���������� / false:���������s</returns>
	bool Init();

	/// <summary>
	/// ���C�����[�v���N��
	/// </summary>
	void Run();

	/// <summary>
	/// �A�v���P�[�V�����̌㏈��
	/// </summary>
	void Terminate();
};

