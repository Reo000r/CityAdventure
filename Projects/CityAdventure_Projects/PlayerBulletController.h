#pragma once
#include "Vector2.h"
#include "Game.h"

#include <vector>
#include <memory>

class PlayerBullet;
class Map;
class GameSceneCamera;

namespace PlayerBulletControllerData
{
	// 弾の最大数
	constexpr int kBulletNum = 6;
}

/// <summary>
/// プレイヤーの弾を管理するクラス
/// </summary>
class PlayerBulletController
{
private:

	int _bulletGraphHandle;

	// 全ての弾を管理
	std::vector<std::shared_ptr<PlayerBullet>> _bulletList;

public:

	PlayerBulletController();
	~PlayerBulletController();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update(std::weak_ptr<Map> map);

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(std::weak_ptr<GameSceneCamera> camera);

	/// <summary>
	/// 条件を満たしていたら弾を追加(発射)する
	/// </summary>
	void AddBullet(Vector2 pos, bool isReverse);

	/// <summary>
	/// 弾と当たったかどうか
	/// </summary>
	/// <returns>当たったらtrue</returns>
	bool IsHitBullet(Game::Rect rect, std::weak_ptr<PlayerBullet>& returnBullet);

};

