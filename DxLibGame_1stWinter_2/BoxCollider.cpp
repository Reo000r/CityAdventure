#include "BoxCollider.h"

#include <DxLib.h>

namespace
{
	// �����蔻��\�����̉~�̔��a
	constexpr int kDispColRad = 4;

	// �����蔻��\�����̐F
	constexpr unsigned int kDispColColor = 0xff00ff;
}

BoxCollider::BoxCollider(Vector2 pos, Vector2 offset, Game::Size size) :
	//Collider(pos, offset, ColliderType::Box, objType, size.width, size.height, sizeMul),
	_pos(pos),
	_colPosOffset(offset),
	_colSize(size),
	_isHitFrame(false)
{

}

void BoxCollider::DispCol(GameSceneCamera camera)
{
	// ���S�ɉ~��\��
	Vector2 circlePos;
	circlePos.x = _pos.x + _colPosOffset.x + camera.GetDrawOffset().x;
	circlePos.y = _pos.y + _colPosOffset.y + camera.GetDrawOffset().y;
	DrawCircle(circlePos.x, circlePos.y, kDispColRad, kDispColColor, false);


	// �����蔻�������

	// �ʒu�Ɠ����蔻�������������`�����擾
	Game::Rect rect = GetColRect();

	// ��`���ɃJ�����̕␳�����������l���擾
	rect = AddCameraOffset(rect, camera);

	// ����A�E��
	Vector2 startPos;
	startPos.x = rect.left;
	startPos.y = rect.top;
	Vector2 endPos;
	endPos.x = rect.right;
	endPos.y = rect.bottom;

	DrawBox(startPos.x, startPos.y, endPos.x, endPos.y, kDispColColor, false);
}
