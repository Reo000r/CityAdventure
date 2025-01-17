#include "PlayerBulletController.h"
#include "PlayerBullet.h"
#include "Map.h"
#include "GameSceneCamera.h"

#include <cassert>

PlayerBulletController::PlayerBulletController()
{
}

PlayerBulletController::~PlayerBulletController()
{
}

void PlayerBulletController::Init()
{
	// 弾のグラフィックをロード
	int bulletGraphHandle = LoadGraph(L"data/img/player/0/Player0_Bullet.png");
	assert(bulletGraphHandle >= 0);

	// 弾を定数の数だけ生成
	for (int i = 0; i < kBulletNum; i++)
	{
		// 生成と初期化、リストへの追加を行う
		// std::shared_ptr<PlayerBullet>
		auto bullet = std::make_shared<PlayerBullet>(bulletGraphHandle);
		bullet->Init();
		_bulletList.push_back(bullet);
	}
}

void PlayerBulletController::Update(std::shared_ptr<Map> map)
{
	for (auto& bullet : _bulletList)
	{
		// 非活性化状態なら飛ばす
		if (!bullet->GetActiveStats()) continue;
		
		bullet->Update(map);
	}
}

void PlayerBulletController::Draw(GameSceneCamera camera)
{
	for (auto& bullet : _bulletList)
	{
		// 非活性化状態なら飛ばす
		if (!bullet->GetActiveStats()) continue;

		bullet->Draw(camera);
	}
}

void PlayerBulletController::AddBullet(Vector2 pos, bool isReverse)
{
	// 非活性化しているものを一つだけ活性化させる
	// 関数を呼びtrue なら成功(活性化できた)
	//           falseなら失敗(活性化状態だったかエラー)
	for (auto& bullet : _bulletList)
	{
		// 弾を発射できた場合は終了
		if (bullet->Active(pos, isReverse))
		{
			break;
		}
	}
}
