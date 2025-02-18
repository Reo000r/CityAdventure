#include "SceneController.h"
#include "TitleScene.h"

SceneController::SceneController()
{
	// ��ԍŏ��̃V�[�������͊��蓖�ĂĂ���
	ChangeScene(std::make_shared<TitleScene>());
}

SceneController& SceneController::GetInstance()
{
	// ���̐錾�̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
	static SceneController controller;
	return controller;
}

void SceneController::Update()
{
	// ��ԏ��Update�̂ݎ��s
	// ���w��Update�͎��s���Ȃ�
	_scenes.back()->Update();
}

void SceneController::Draw()
{
	// Draw�͑S�Ď��s
	for (auto& scene : _scenes)
	{
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	if (_scenes.empty())
	{
		// �󂾂����ꍇ�͐擪�ɒu��
		_scenes.push_back(scene);
	}
	else
	{
		// �Ȃɂ�����ꍇ�͍Ō�̗v�f��u��������
		_scenes.back() = scene;
	}

	// �����bgm���Đ�
	for (auto& scene : _scenes)
	{
		scene->StartBGM();
		break;
	}
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	// �Ō���ɒǉ�
	_scenes.push_back(scene);
}

void SceneController::PopScene()
{
	// ���s���̃V�[����1�ȉ��Ȃ�
	if (_scenes.size() <= 1)
	{
		// ���s���ׂ��V�[�����Ȃ��Ȃ邽�ߋ����Ȃ�
		return;
	}

	// �Ō������菜��
	_scenes.pop_back();
}
