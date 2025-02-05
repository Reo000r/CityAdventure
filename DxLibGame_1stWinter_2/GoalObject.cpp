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

	// �A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (_animFrameCount >= GoalData::kAnimTotalFrame)
	{
		_animFrameCount = 0;
		_isReverseGraphX = !_isReverseGraphX;
	}
}

void GoalObject::Draw(std::weak_ptr<GameSceneCamera> camera) const
{
	// �O���t�B�b�N�̐؂�o���ʒu(X���W)���v�Z����
	int animNo = _animFrameCount / GoalData::kSingleAnimFrame;

	// �\���ʒu(���S)
	int x = static_cast<int>(_pos.x + _drawPosOffset.x + camera.lock()->GetDrawOffset().x);
	int y = static_cast<int>(_pos.y + _drawPosOffset.y + camera.lock()->GetDrawOffset().y);

	// �؂�o���J�n�ʒu
	int cutX = animNo * GoalData::kGraphWidth;
	int cutY = 0;

	// �؂�o����
	int width = GoalData::kGraphWidth;
	int height = GoalData::kGraphHeight;

	// �\���{��
	int sizeMul = GoalColData::kSizeMul;

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		sizeMul, 0.0,
		_graphHandle, true, _isReverseGraphX, false);

#ifdef _DEBUG

	// �f�o�b�O�`��
	DispCol(camera);
	
#endif // _DEBUG
}
