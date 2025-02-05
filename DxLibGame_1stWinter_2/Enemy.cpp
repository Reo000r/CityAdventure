#include "Enemy.h"

Enemy::Enemy(Vector2 pos, Vector2 offset, Game::Size size, int hitPoint) :
	BoxCollider(pos, offset, size),
	_spawnPos(pos),
	_animFrameCount(0),
	_hitPoint(hitPoint),
	_isReverseGraphX(false),
	_isActive(false),
	_vel({0.0f, 0.0f})
{
}
