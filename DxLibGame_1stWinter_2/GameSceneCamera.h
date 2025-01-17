#pragma once
#include "Vector2.h"

class Player;

class GameSceneCamera
{
private:
	// カメラのポジション
	Vector2 _pos;

	// 描画するときに使う補正値
	Vector2 _drawOffset;

	// プレイヤーを基準に動かす
	Player& _player;

public:
	GameSceneCamera(Player& player);

	void Update();

	Vector2 GetDrawOffset() const { return _drawOffset; }
};