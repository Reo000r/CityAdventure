#include "MapChip.h"
#include "GameSceneCamera.h"

MapChip::MapChip(Vector2 pos, int mapChipNum) :
	BoxCollider(pos, { 0.0f, 0.0f }, { MapColData::kColSizeX, MapColData::kColSizeY }),
	_mapChipNum(mapChipNum)
{
	
}

void MapChip::Init()
{

}

void MapChip::Update()
{
}

void MapChip::Draw(std::weak_ptr<GameSceneCamera> camera, int mapGraphHandle) const
{
	// 表示しない番号なら描画処理なし
	if (_mapChipNum == Game::kEmptyChipNum) return;

	// 表示位置
	Vector2 pos = _pos + camera.lock()->GetDrawOffset();

	// 切り出し位置、切り出す量
	Vector2 cutPos  = GetPosInGraph();
	Game::Size size = GetChipGraphSize();

	// 表示倍率
	int sizeMul = MapColData::kSizeMul;

	// 描画
	DrawRectRotaGraph(
		pos.x, pos.y,             // 表示位置
		cutPos.x, cutPos.y,       // 切り出し開始位置
		size.width, size.height,  // 切り出し量
		sizeMul, 0.0,             // 表示倍率、回転量
		mapGraphHandle, true,
		false, false);

#ifdef _DEBUG

	// デバッグ描画
	DispCol(camera);
	DrawFormatString(pos.x, pos.y, 0xffffff, L"%d", _mapChipNum);
	//DrawFormatString(pos.x - 32, pos.y - 8, 0xffffff, L"%.00f/%.00f", _pos.x, _pos.y);

#endif // _DEBUG
}