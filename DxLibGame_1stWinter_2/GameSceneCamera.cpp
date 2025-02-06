#include "GameSceneCamera.h"
#include "Player.h"

#include "Vector2.h"
#include "Game.h"
#include <DxLib.h>

namespace
{
	// プレイヤーを追尾し始める範囲
	constexpr int kCameraMoveRangeX = 40;
	constexpr int kCameraMoveRangeY = 50;

	// プレイヤーを追う限界距離(上下)
	constexpr int kCameraMaxRangeTop = 512 - 128;
	constexpr int kCameraMaxRangeBottom = 880 + 1024 + 32*2*3;
	// プレイヤーを追う限界距離(左右)
	constexpr int kCameraMaxRangeRight = 5725;// - 2500;
	constexpr int kCameraMaxRangeLeft = 620;


	// Lerpを行う倍率
	// 高ければ高いほどすぐに追いつく
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
	// 目標となる位置
	Vector2 targetPos = _pos;

	// プレイヤーの位置
	Vector2 playerPos = _player.GetPos();
	//printfDx(L"%f / %f\n", playerPos.x, playerPos.y);

	// カメラから見てプレイヤーが決められた範囲外に出ようとしていたら
	// 補正をかけつつ追いかける
	// (kCameraMoveRangeX/Y * 0.5f) が許容範囲
	if (playerPos.x > _pos.x + (kCameraMoveRangeX * 0.5f))
	{
		// 右
		targetPos.x = playerPos.x - (kCameraMoveRangeX * 0.5f);
	}
	else if (playerPos.x < _pos.x - (kCameraMoveRangeX * 0.5f))
	{
		// 左
		targetPos.x = playerPos.x + (kCameraMoveRangeX * 0.5f);
	}

	if (playerPos.y > _pos.y + (kCameraMoveRangeY * 0.5f))
	{
		// 上
		targetPos.y = playerPos.y - (kCameraMoveRangeY * 0.5f);
	}
	else if (playerPos.y < _pos.y - (kCameraMoveRangeY * 0.5f))
	{
		// 下
		targetPos.y = playerPos.y + (kCameraMoveRangeY * 0.5f);
	}

	// カメラと目標地点のベクトルの長さを
	// 第三引数倍してカメラの位置に代入
	// (少しプレイヤーの方に向かうような挙動)
	_pos += _pos.Lerp(_pos, targetPos, kCameraLerpMul);
	//_pos.x = std::lerp(_pos.x, targetPos.x, kCameraLerpMul);
	//_pos.y = std::lerp(_pos.y, targetPos.y, kCameraLerpMul);
	//_pos = playerPos;

	//printfDx(L"%f / %f\n", _pos.x, _pos.y);

	// カメラの位置が一定範囲を超えたら範囲内に補正
	if (_pos.y < kCameraMaxRangeTop)
	{
		// 上
		_pos.y = kCameraMaxRangeTop;
	}
	if (_pos.y > kCameraMaxRangeBottom)
	{
		// 下
		_pos.y = kCameraMaxRangeBottom;
	}

	if (_pos.x < kCameraMaxRangeLeft)
	{
		// 左
		_pos.x = kCameraMaxRangeLeft;
	}
	if (_pos.x > kCameraMaxRangeRight)
	{
		// 右
		_pos.x = kCameraMaxRangeRight;
	}

	// _drawOffsetを調整していく
	// 直感的に書けるように反転させる
	_drawOffset = _pos * -1;

	// プレイヤーを中心にする
	// (カメラの位置が中央に来るようにしている)
	_drawOffset.x += Game::kScreenCenterWidth;
	_drawOffset.y += Game::kScreenCenterHeight;

	//DrawFormatString(96, 144, 0xffffff, L"CameraPos : %f/%f", _pos.x, _pos.y);
}