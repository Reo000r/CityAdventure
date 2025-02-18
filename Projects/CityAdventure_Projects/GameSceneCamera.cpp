#include "GameSceneCamera.h"
#include "Player.h"

#include "Vector2.h"
#include "Game.h"
#include <DxLib.h>

namespace
{
	// �v���C���[��ǔ����n�߂�͈�
	constexpr int kCameraMoveRangeX = 40;
	constexpr int kCameraMoveRangeY = 50;

	// �v���C���[��ǂ����E����(�㉺)
	constexpr int kCameraMaxRangeTop = 512 - 128;
	constexpr int kCameraMaxRangeBottom = 880 + 1024 + 32*2*3;
	// �v���C���[��ǂ����E����(���E)
	constexpr int kCameraMaxRangeRight = 5725;
	constexpr int kCameraMaxRangeLeft = 620;


	// Lerp���s���{��
	// ������΍����قǂ����ɒǂ���
	constexpr float kCameraLerpMul = 0.5f;
}

GameSceneCamera::GameSceneCamera(Player& player) :
	_player(player),
	_pos(0.0f, 0.0f),
	_drawOffset(0.0f, 0.0f)
{

}

void GameSceneCamera::Update()
{
	// �ڕW�ƂȂ�ʒu
	Vector2 targetPos = _pos;

	// �v���C���[�̈ʒu
	Vector2 playerPos = _player.GetPos();

	// �J�������猩�ăv���C���[�����߂�ꂽ�͈͊O�ɏo�悤�Ƃ��Ă�����
	// �␳�������ǂ�������
	// (kCameraMoveRangeX/Y * 0.5f) �����e�͈�
	if (playerPos.x > _pos.x + (kCameraMoveRangeX * 0.5f))
	{
		// �E
		targetPos.x = playerPos.x - (kCameraMoveRangeX * 0.5f);
	}
	else if (playerPos.x < _pos.x - (kCameraMoveRangeX * 0.5f))
	{
		// ��
		targetPos.x = playerPos.x + (kCameraMoveRangeX * 0.5f);
	}

	if (playerPos.y > _pos.y + (kCameraMoveRangeY * 0.5f))
	{
		// ��
		targetPos.y = playerPos.y - (kCameraMoveRangeY * 0.5f);
	}
	else if (playerPos.y < _pos.y - (kCameraMoveRangeY * 0.5f))
	{
		// ��
		targetPos.y = playerPos.y + (kCameraMoveRangeY * 0.5f);
	}

	// �J�����ƖڕW�n�_�̃x�N�g���̒�����
	// ��O�����{���ăJ�����̈ʒu�ɑ��
	// (�����v���C���[�̕��Ɍ������悤�ȋ���)
	_pos += _pos.Lerp(_pos, targetPos, kCameraLerpMul);

	// �J�����̈ʒu�����͈͂𒴂�����͈͓��ɕ␳
	if (_pos.y < kCameraMaxRangeTop)
	{
		// ��
		_pos.y = kCameraMaxRangeTop;
	}
	if (_pos.y > kCameraMaxRangeBottom)
	{
		// ��
		_pos.y = kCameraMaxRangeBottom;
	}

	if (_pos.x < kCameraMaxRangeLeft)
	{
		// ��
		_pos.x = kCameraMaxRangeLeft;
	}
	if (_pos.x > kCameraMaxRangeRight)
	{
		// �E
		_pos.x = kCameraMaxRangeRight;
	}

	// _drawOffset�𒲐����Ă���
	// �����I�ɏ�����悤�ɔ��]������
	_drawOffset = _pos * -1;

	// �v���C���[�𒆐S�ɂ���
	// (�J�����̈ʒu�������ɗ���悤�ɂ��Ă���)
	_drawOffset.x += Game::kScreenCenterWidth;
	_drawOffset.y += Game::kScreenCenterHeight;
}