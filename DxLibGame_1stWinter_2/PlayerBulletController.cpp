#include "PlayerBulletController.h"
#include "PlayerBullet.h"
#include "Map.h"
#include "GameSceneCamera.h"

#include <cassert>

PlayerBulletController::PlayerBulletController() :
	_bulletGraphHandle(0)
{
}

PlayerBulletController::~PlayerBulletController()
{
	DeleteGraph(_bulletGraphHandle);
}

void PlayerBulletController::Init()
{
	// 弾のグラフィックをロード
	_bulletGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_Bullet.png");
	assert(_bulletGraphHandle >= 0);

	// 弾を定数の数だけ生成
	for (int i = 0; i < kBulletNum; i++)
	{
		// 生成と初期化、リストへの追加を行う
		// std::shared_ptr<PlayerBullet>
		auto bullet = std::make_shared<PlayerBullet>(_bulletGraphHandle);
		bullet->Init();
		_bulletList.push_back(bullet);
	}
}

void PlayerBulletController::Update(std::weak_ptr<Map> map)
{
	for (auto& bullet : _bulletList)
	{
		// 非活性化状態なら飛ばす
		if (!bullet->GetActiveStats()) continue;
		
		bullet->Update(map);
	}
}

void PlayerBulletController::Draw(std::weak_ptr<GameSceneCamera> camera)
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

bool PlayerBulletController::IsHitBullet(Game::Rect rect, std::weak_ptr<PlayerBullet>& returnBullet)
{
	// 全ての弾と当たり判定を行う
	for (auto& bullet : _bulletList)
	{
		// 非活性化状態なら飛ばす
		if (!bullet->GetActiveStats()) continue;

		// 弾の当たり判定を取得
		Game::Rect bulletRect = bullet->GetRect();

		// 弾と当たり判定
		bool isHit = !(
			(rect.top >= bulletRect.bottom || rect.bottom <= bulletRect.top) ||
			(rect.left >= bulletRect.right || rect.right <= bulletRect.left));
		// 当たっていたら
		if (isHit)
		{
			returnBullet = bullet;
			// 弾を非活性化
			bullet->DisActive();
			return true;
		}
	}

	// 当たっていなかったら
	return false;
}