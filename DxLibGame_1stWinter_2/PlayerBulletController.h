#pragma once
#include "Vector2.h"
//#include "PlayerBullet.h"

#include <vector>
#include <memory>

class PlayerBullet;
class Map;
class GameSceneCamera;

namespace
{
	constexpr int kBulletNum = 50;
}

class PlayerBulletController
{
private:

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
	void Update(std::shared_ptr<Map> map);

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw(GameSceneCamera camera);

	/// <summary>
	/// 条件を満たしていたら弾を追加(発射)する
	/// </summary>
	void AddBullet(Vector2 pos, bool isReverse);

};

