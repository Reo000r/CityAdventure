#include "UIController.h"
#include "PlayerHitpointUI.h"
#include "Player.h"

#include <cassert>

UIController::UIController() :
	_playerUndamagedHitpointGraphHandle(0),
	_playerDamagedHitpointGraphHandle(0)
{
}

UIController::~UIController()
{
	DeleteGraph(_playerUndamagedHitpointGraphHandle);
	DeleteGraph(_playerDamagedHitpointGraphHandle);
}

void UIController::Init(std::weak_ptr<Player> player)
{
	_player = player;

	_playerUndamagedHitpointGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_UndamagedHitPoint.png");
	assert(_playerUndamagedHitpointGraphHandle >= 0);
	_playerDamagedHitpointGraphHandle = LoadGraph(L"data/img/player/normal/NormalPlayer_DamagedHitPoint.png");
	assert(_playerDamagedHitpointGraphHandle >= 0);

	int maxHp = _player.lock()->GetMaxHitpoint();
	Vector2 pos = {8*3+16/2, 8*3+16/2};

	for (int i = 1; i <= maxHp; i++)
	{
		auto hpUI = std::make_shared<PlayerHitpointUI>(pos, i);
		hpUI->Init(_player, _playerUndamagedHitpointGraphHandle, _playerDamagedHitpointGraphHandle);
		_hpList.push_back(hpUI);
		pos.x += (PlayerHitpointUIData::kUndamageGraphWidth + 1) * PlayerHitpointUIData::kUndamageSizeMul;
	}

}

void UIController::Update()
{
	for (auto& hpUI : _hpList)
	{
		hpUI->Update();
	}
}

void UIController::Draw(std::weak_ptr<GameSceneCamera> camera)
{
	for (auto& hpUI : _hpList)
	{
		hpUI->Draw();
	}
}
