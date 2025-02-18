#include "EnemyBulletController.h"
#include "EnemyBullet.h"
#include "Map.h"
#include "GameSceneCamera.h"

#include <cassert>

EnemyBulletController::EnemyBulletController() :
	_bulletGraphHandle(0)
{
}

EnemyBulletController::~EnemyBulletController()
{
	DeleteGraph(_bulletGraphHandle);
}

void EnemyBulletController::Init(std::weak_ptr<Player> player)
{
	_player = player;

	// 弾のグラフィックをロード
	_bulletGraphHandle = LoadGraph(L"data/img/enemy/bossenemy/BossEnemy_Bullet.png");
	assert(_bulletGraphHandle >= 0);

	// 弾を定数の数だけ生成
	for (int i = 0; i < EnemyBulletControllerData::kBulletNum; i++)
	{
		// 生成と初期化、リストへの追加を行う
		// std::shared_ptr<PlayerBullet>
		auto bullet = std::make_shared<EnemyBullet>(_bulletGraphHandle);
		bullet->Init(_player);
		_bulletList.push_back(bullet);
	}
}

void EnemyBulletController::Update(std::weak_ptr<Map> map)
{
	for (auto& bullet : _bulletList)
	{
		// 非活性化状態なら飛ばす
		if (!bullet->GetActiveStats()) continue;

		bullet->Update(map);
	}
}

void EnemyBulletController::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	for (auto& bullet : _bulletList)
	{
		// 非活性化状態なら飛ばす
		if (!bullet->GetActiveStats()) continue;

		bullet->Draw(camera);
	}
}

void EnemyBulletController::AddBullet(Vector2 pos, bool isReverse)
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

bool EnemyBulletController::IsHitBullet(Game::Rect rect, std::weak_ptr<EnemyBullet>& returnBullet)
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