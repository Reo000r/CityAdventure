#pragma once
#include "Vector2.h"

class Player;

/// <summary>
/// �Q�[�����̃J�����������Ǘ�����
/// �V�[��������shared_ptr�������A����weak_ptr���������Ȃ�
/// </summary>
class GameSceneCamera
{
private:
	// �J�����̃|�W�V����
	Vector2 _pos;

	// �`�悷��Ƃ��Ɏg���␳�l
	Vector2 _drawOffset;

	// �v���C���[����ɓ�����
	Player& _player;

public:
	GameSceneCamera(Player& player);

	void Update();

	Vector2 GetDrawOffset() const { return _drawOffset; }
};