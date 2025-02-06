#include "GoalObject.h"

#include <cassert>

GoalObject::GoalObject() :
	BoxCollider({ GoalData::kStartPosX, GoalData::kStartPosY }, {}, { GoalColData::kColWidth, GoalColData::kColHeight }),
	_drawPosOffset(GoalData::kDrawPosOffsetX, GoalData::kDrawPosOffsetY),
	_graphHandle(0),
	_animFrameCount(0),
	_isReverseGraphX(false),
	_isActive(false),
	_alpha(0.0f)
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
	if (!_isActive) return;

	if (_alpha < 1.0f)
	{
		_alpha += 1 / (60.0f * 2);
		if (_alpha > 1.0f) _alpha = 1.0f;
	}

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
	if (!_isActive) return;

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


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * _alpha));
	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		sizeMul, 0.0,
		_graphHandle, true, _isReverseGraphX, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG

	// �f�o�b�O�`��
	DispCol(camera);
	
#endif // _DEBUG
}

bool GoalObject::Active()
{
	if (_isActive) return false;

	_isActive = true;
	_alpha = 0.0f;

	return true;
}
