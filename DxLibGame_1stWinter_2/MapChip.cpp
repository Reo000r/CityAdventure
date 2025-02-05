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
	// �\�����Ȃ��ԍ��Ȃ�`�揈���Ȃ�
	if (_mapChipNum == Game::kEmptyChipNum) return;

	// �\���ʒu
	Vector2 pos = _pos + camera.lock()->GetDrawOffset();

	// �؂�o���ʒu�A�؂�o����
	Vector2 cutPos  = GetPosInGraph();
	Game::Size size = GetChipGraphSize();

	// �\���{��
	int sizeMul = MapColData::kSizeMul;

	// �`��
	DrawRectRotaGraph(
		pos.x, pos.y,             // �\���ʒu
		cutPos.x, cutPos.y,       // �؂�o���J�n�ʒu
		size.width, size.height,  // �؂�o����
		sizeMul, 0.0,             // �\���{���A��]��
		mapGraphHandle, true,
		false, false);

#ifdef _DEBUG

	// �f�o�b�O�`��
	DispCol(camera);
	DrawFormatString(pos.x, pos.y, 0xffffff, L"%d", _mapChipNum);
	//DrawFormatString(pos.x - 32, pos.y - 8, 0xffffff, L"%.00f/%.00f", _pos.x, _pos.y);

#endif // _DEBUG
}