#include "UIBase.h"

UIBase::UIBase(Vector2 pos) :
	_useHandle(0),
	_animFrameCount(0),
	_isReverseGraphX(false),
	_isActive(false),
	_alpha(1),
	_pos(pos),
	_vel({})
{
}
