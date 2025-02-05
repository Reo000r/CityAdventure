#include "GoalObject.h"

#include <cassert>

GoalObject::GoalObject() :
	BoxCollider({ GoalData::kStartPosX, GoalData::kStartPosY }, {}, { GoalColData::kColWidth, GoalColData::kColHeight }),
	_drawPosOffset(GoalData::kDrawPosOffsetX, GoalData::kDrawPosOffsetY),
	_graphHandle(0),
	_animFrameCount(0),
	_isReverseGraphX(false)
{
}

GoalObject::~GoalObject()
{
	DeleteGraph(_graphHandle);
}

void GoalObject::Init(int graphHandle)
{
	_graphHandle = graphHandle;
	assert(_graphHandle >= 0);
}

void GoalObject::Update()
{
	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= GoalData::kAnimTotalFrame)
	{
		_animFrameCount = 0;
		_isReverseGraphX = !_isReverseGraphX;
	}
}

void GoalObject::Draw(std::weak_ptr<GameSceneCamera> camera) const
{
	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / GoalData::kSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// 切り出し開始位置
	int cutX = animNo * GoalData::kGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width = GoalData::kGraphWidth;
	int height = GoalData::kGraphHeight;

	// 表示倍率
	int sizeMul = GoalColData::kSizeMul;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		sizeMul, 0.0,
		_graphHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	// デバッグ描画
	DispCol(camera);
	
#endif // _DEBUG
}
