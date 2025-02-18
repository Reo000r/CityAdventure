#include "BoxCollider.h"

#include <DxLib.h>

namespace
{
	// 当たり判定表示時の円の半径
	constexpr int kDispColRad = 4;

	// 当たり判定表示時の色
	constexpr unsigned int kDispColColor = 0xff00ff;
}

BoxCollider::BoxCollider(Vector2 pos, Vector2 offset, Game::Size size) :
	_pos(pos),
	_colPosOffset(offset),
	_colSize(size)
{

}

void BoxCollider::DispCol(std::weak_ptr<GameSceneCamera> camera) const
{
	// 中心に円を表示
	Vector2 circlePos;
	circlePos.x = _pos.x + _colPosOffset.x + camera.lock()->GetDrawOffset().x;
	circlePos.y = _pos.y + _colPosOffset.y + camera.lock()->GetDrawOffset().y;
	DrawCircle(circlePos.x, circlePos.y, kDispColRad, kDispColColor, false);


	// 当たり判定を可視化

	// 位置と当たり判定を加味した矩形情報を取得
	Game::Rect rect = GetColRect();

	// 矩形情報にカメラの補正を加味した値を取得
	rect = AddCameraOffset(rect, camera);

	// 左上、右下
	Vector2 startPos;
	startPos.x = rect.left;
	startPos.y = rect.top;
	Vector2 endPos;
	endPos.x = rect.right;
	endPos.y = rect.bottom;

	DrawBox(startPos.x, startPos.y, endPos.x, endPos.y, kDispColColor, false);
}
