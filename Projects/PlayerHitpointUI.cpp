#include "PlayerHitpointUI.h"
#include "Player.h"

void PlayerHitpointUI::UndamageUpdate()
{
	if (!_isActive) return;

	_playerCurrentHitpoint = _player.lock()->GetCurrentHitpoint();

	// state切替
	if (_hitpointIndex > _playerCurrentHitpoint)
	{
		_nowUpdateState = &PlayerHitpointUI::OndamageUpdate;
		_drawPosOffset = PlayerHitpointUIData::kDamageDrawPosOffset;
		_animFrameCount = 0;
		return;
	}

	_animFrameCount++;

	// アニメーションの合計フレーム数を超えたら最初に戻す
	if (_animFrameCount >= PlayerHitpointUIData::kUndamageAnimTotalFrame)
	{
		_animFrameCount = 0;
	}
}

void PlayerHitpointUI::OndamageUpdate()
{
	_damageEffectCount++;

	if (_damageEffectCount % 4 == 0)
	{
		int vibrationWidth = 4;
		_vel = 
		{ static_cast<float>(GetRand(vibrationWidth*2) - vibrationWidth),
			static_cast<float>(GetRand(vibrationWidth*2) - vibrationWidth) };
	}
	
	// state切替
	if (_damageEffectCount >= PlayerHitpointUIData::kDamageEffectAnimFrame)
	{
		_nowUpdateState = &PlayerHitpointUI::DamageUpdate;
		_drawPosOffset = PlayerHitpointUIData::kDamageDrawPosOffset;
		_useHandle = _damagedGraphHandle;
		_animFrameCount = 0;
		_vel = {};
		return;
	}
}

void PlayerHitpointUI::DamageUpdate()
{
}

PlayerHitpointUI::PlayerHitpointUI(Vector2 pos, int hitpointIndex) :
	UIBase(pos),
	_nowUpdateState(&PlayerHitpointUI::UndamageUpdate),
	_undamagedGraphHandle(0),
	_damagedGraphHandle(0),
	_hitpointIndex(hitpointIndex),
	_playerMaxHitpoint(0),
	_playerCurrentHitpoint(0),
	_damageEffectCount(0),
	_drawPosOffset(PlayerHitpointUIData::kUndamageDrawPosOffset)
{
}

void PlayerHitpointUI::Init(std::weak_ptr<Player> player, int undamagedGraphHandle, int damagedGraphHandle)
{
	_player = player;
	_undamagedGraphHandle = undamagedGraphHandle;
	_damagedGraphHandle   = damagedGraphHandle;
	_useHandle = _undamagedGraphHandle;

	_playerMaxHitpoint = _player.lock()->GetMaxHitpoint();
	_playerCurrentHitpoint = _player.lock()->GetCurrentHitpoint();

	Active(_pos, false);
}

void PlayerHitpointUI::Update()
{
	(this->*_nowUpdateState)();
}

void PlayerHitpointUI::Draw()
{
	if (!_isActive) return;

	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _animFrameCount / PlayerHitpointUIData::kUndamageSingleAnimFrame;
	
	// 表示位置(中心)
	int x = static_cast<int>(_pos.x + _vel.x + _drawPosOffset.x);
	int y = static_cast<int>(_pos.y + _vel.y + _drawPosOffset.y);

	// 切り出し開始位置
	int cutX = animNo * PlayerHitpointUIData::kUndamageGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width  = PlayerHitpointUIData::kUndamageGraphWidth;
	int height = PlayerHitpointUIData::kUndamageGraphHeight;

	float sizeMul = PlayerHitpointUIData::kUndamageSizeMul;

	if (_nowUpdateState == &PlayerHitpointUI::DamageUpdate)
	{
		animNo = _animFrameCount / PlayerHitpointUIData::kDamageSingleAnimFrame;
		cutX = animNo * PlayerHitpointUIData::kDamageGraphWidth;
		width = PlayerHitpointUIData::kDamageGraphWidth;
		height = PlayerHitpointUIData::kDamageGraphHeight;
		sizeMul = PlayerHitpointUIData::kDamageSizeMul;
	}

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		sizeMul, 0.0,
		_useHandle, true, _isReverseGraphX, false);
}

bool PlayerHitpointUI::Active(Vector2 pos, bool isReverse)
{
	if (_isActive) return false;

	_isActive = true;
	_pos = pos;
	_isReverseGraphX = isReverse;

	return true;
}

void PlayerHitpointUI::DisActive()
{
}
