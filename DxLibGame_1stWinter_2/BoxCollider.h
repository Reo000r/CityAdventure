#pragma once
#include "Vector2.h"
#include "Game.h"
#include "GameSceneCamera.h"

// 元はColliderクラスを継承していたが、
// CircleColliderの実装予定が無くなったので
// Colliderに持たせていたものを直接持つようになった
class BoxCollider
{
protected:

	// オブジェクトの中心位置
	Vector2 _pos;

	// 当たり判定作成時の位置調整用
	Vector2 _colPosOffset;

	// 判定の幅と高さ
	// (表示倍率は含む)
	Game::Size _colSize;

	// このフレームでヒットしていたかどうか
	// 各Update関数内の最初でfalseにしておく
	// (いらないかも)
	bool _isHitFrame;

public:
	BoxCollider(Vector2 pos, Vector2 offset, Game::Size size);

	/// <summary>
	/// 当たり判定の表示と
	/// オブジェクトの中心に円の表示
	/// </summary>
	/// <param name="camera"></param>
	void DispCol(GameSceneCamera camera);

	/// <summary>
	/// 大きさの倍率を含んだ判定の幅と高さ情報を返す
	/// </summary>
	/// <returns>大きさの倍率を含んだ判定の幅と高さ情報</returns>
	Game::Size GetBoxSize() { return _colSize; }

	/// <summary>
	/// 位置を加味した矩形情報を返す
	/// 大きさの倍率は含む
	/// 当たり判定の位置補正は考慮しない
	/// (位置を加味しないほうが使いやすい？)
	/// </summary>
	/// <returns>位置を加味した矩形情報</returns>
	Game::Rect GetRect() const
	{
		Game::Rect rect;

		rect.top    = static_cast<int>(_pos.y - _colSize.height * 0.5f);
		rect.bottom = static_cast<int>(_pos.y + _colSize.height * 0.5f);
		rect.left   = static_cast<int>(_pos.x - _colSize.width  * 0.5f);
		rect.right  = static_cast<int>(_pos.x + _colSize.width  * 0.5f);
		
		return rect;
	}

	/// <summary>
	/// 位置と当たり判定を加味した矩形情報を返す
	/// 大きさの倍率は含む
	/// colPosOffsetを計算に追加
	/// </summary>
	/// <returns>位置と当たり判定を加味した矩形情報</returns>
	Game::Rect GetColRect() const
	{
		Game::Rect rect;

		rect.top    = static_cast<int>(_pos.y - _colSize.height * 0.5f + _colPosOffset.y);
		rect.bottom = static_cast<int>(_pos.y + _colSize.height * 0.5f + _colPosOffset.y);
		rect.left   = static_cast<int>(_pos.x - _colSize.width  * 0.5f + _colPosOffset.x);
		rect.right  = static_cast<int>(_pos.x + _colSize.width  * 0.5f + _colPosOffset.x);
		
		return rect;
	}

	/// <summary>
	/// 矩形情報にカメラの補正を加味した値を返す
	/// 大きさの倍率は含む
	/// </summary>
	/// <returns>位置と当たり判定を加味した矩形情報</returns>
	Game::Rect AddCameraOffset(Game::Rect rect, const GameSceneCamera camera)
	{
		rect.top    += static_cast<int>(camera.GetDrawOffset().y);
		rect.bottom += static_cast<int>(camera.GetDrawOffset().y);
		rect.left   += static_cast<int>(camera.GetDrawOffset().x);
		rect.right  += static_cast<int>(camera.GetDrawOffset().x);
		
		return rect;
	}
};

