#pragma once
#include "BoxCollider.h"

#include <memory>

class Map;
class GameSceneCamera;
class Player;
class PlayerBulletController;

/// <summary>
/// 敵の基底クラス
/// </summary>
class Enemy : public BoxCollider
{
protected:
	int _animFrameCount;

	// 耐久力
	int _hitPoint;
	// 左ならtrue、右ならfalse
	bool _isReverseGraphX;

	// trueなら活性化
	// UpdateとDrawとDisActiveを受け付ける
	// falseなら非活性化
	// Activeを受け付ける
	bool _isActive;

	// _posは中心
	Vector2 _vel;

	// 初期位置保存
	const Vector2 _spawnPos;

	std::weak_ptr<Map> _map;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">中心位置</param>
	/// <param name="offset">当たり判定作成時の位置補正値</param>
	/// <param name="size">表示倍率を含む判定の幅と高さ</param>
	Enemy(Vector2 pos, Vector2 offset, Game::Size size, int hitPoint);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(std::weak_ptr<Map> map, 
		std::weak_ptr<Player> player, 
		std::weak_ptr<PlayerBulletController> playerBulletController, 
		int idleHandle, 
		int runHandle, 
		int fallHandle) abstract;

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 描画全般
	/// </summary>
	virtual void Draw(std::weak_ptr<GameSceneCamera> camera) abstract;

	/// <summary>
	/// 活性化
	/// </summary>
	virtual bool Active(Vector2 pos, bool isReverse) abstract;

	/// <summary>
	/// 非活性化
	/// </summary>
	virtual void DisActive() abstract;

	/// <summary>
	/// ダメージを食らった時の処理
	/// </summary>
	virtual void OnDamage(int damage, bool isReverseX) abstract;

	/// <summary>
	/// 活性化状態を取得
	/// </summary>
	bool GetActiveStats() const { return _isActive; }
};
