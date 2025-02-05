#include "Map.h"
#include "Game.h"
#include "GameSceneCamera.h"
#include "BoxCollider.h"
#include "PlatinumLoader.h"

#include <DxLib.h>
#include <cassert>

namespace
{
	constexpr unsigned int kBackGroundColor = 0x333333;
}

Map::Map() :
	_mapGraphHandle(0)
{
	PlatinumLoader& loader = PlatinumLoader::GetInstance();

	//loader.Load(L"data/mapdata/stage1.fmf");
	loader.Load(L"data/mapdata/stage.fmf");

	int mapWidth = 0;
	int mapHeight = 0;
	// �}�b�v�f�[�^�ɒu����Ă���`�b�v���c���ɂ����u����Ă���̂������
	loader.GetMapSize(mapWidth, mapHeight);

	// ������ꍇ�͔͈͊O�ǂݎ������Ă��܂��\��������̂Œe��
	assert(MapGraphData::kMapSizeX == mapWidth);
	assert(MapGraphData::kMapSizeY == mapHeight);

	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			// �}�b�v�f�[�^���}�b�v�`�b�v�ɓK�p

			int mapChipNum = loader.GetChipSpriteNo(LayerType::BackGround, chipX, chipY);

			// ���ۂɒu�����ʒu
			Vector2 pos;
			pos.x = chipX * MapColData::kColSizeX;
			pos.y = chipY * MapColData::kColSizeY;

			// �|�C���^���쐬���ۑ�
			auto chip = std::make_shared<MapChip>(pos, mapChipNum);
			//auto chip = std::make_shared<MapChip>(pos, posInGraph, drawSize, colSize, mapChipNum, MapData::kSizeMul);
			_mapChip[chipY][chipX] = chip;
			//_mapChip[chipY][chipX]->Init();
		}
	}
}

void Map::Init(int mapGraphHandle, std::weak_ptr<GameSceneCamera> camera)
{
	_mapGraphHandle = mapGraphHandle;
	_camera = camera;
}

void Map::Draw() const
{
	//DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBackGroundColor, true);

	for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
	{
		for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
		{
			int chipNo = _mapChip[chipY][chipX]->GetMapChipNum();

			// �����Ȃ玟��
			if (chipNo == Game::kEmptyChipNum) continue;

			_mapChip[chipY][chipX]->Draw(_camera, _mapGraphHandle);
		}
	}
}

Vector2 Map::CheckHitAllMapChip(Vector2 pos, const Vector2 vel, const Game::Size size) const
{
	// �ŏ�����0�Ȃ�return
	if (vel.Magnitude() == 0.0f)
	{
		return vel;
	}

	Vector2 resultVel = vel;

	// ������Ȃ��Ȃ�܂ŌJ��Ԃ�
	bool loop = true;
	// ����œ���������
	bool isFirstHit = true;
	while (loop)
	{
		loop = false;

		// �����̈ʒu���o��
		Vector2 futurePos = pos + resultVel;

		// �S�}�b�v�`�b�v���J��Ԃ�
		//for (int chipX = 0; chipX < MapData::kMapSizeX; chipX++)
		for (int chipY = 0; chipY < MapGraphData::kMapSizeY; chipY++)
		{
			bool isHit = false;

			//for (int chipY = 0; chipY < MapData::kMapSizeY; chipY++)
			for (int chipX = 0; chipX < MapGraphData::kMapSizeX; chipX++)
			{
				// �ړ��x�N�g���̒������Ȃ�������
				if (resultVel.Magnitude() == 0.0f)
				{
					return resultVel;
				}

				// �����蔻������O��
				// �����I�ɐ�Γ�����Ȃ������m�肵�Ă���`�b�v��
				// ���̎��_��continue������
				Vector2 chipPos = GetMapChipData(chipX, chipY)->GetPos();
				if (futurePos.Distance(futurePos, chipPos) > 800)
				{
					continue;
				}

				isHit = CheckHitMapChip(futurePos, size, chipX, chipY);

				// ����ɓ��������Ƃ�
				if (isHit && isFirstHit)
				{
					// ���㓖���蔻��Ń|�W�V������velocity�̕␳������Ƃ��A�����_�ȉ��̌덷���Y�܂��
					// �G��1�h�b�g�����炷�A���w�v�Z�����Ȃ��}�b�V�u�����蔻��ɂ͎ז��Ȃ̂ŏ���Ɋۂ߂Ă��܂��A
					// �ȍ~���߂ē����蔻��
					// pos��vel���ۂ߂�
					futurePos.x = floorf(futurePos.x);
					futurePos.y = floorf(futurePos.y);
					resultVel.x = floorf(resultVel.x);
					resultVel.y = floorf(resultVel.y);
					isFirstHit = false;

					// ���[�v�p��
					loop = true;
				}

				// �����������_�Ń}�b�v�`�b�v�̃��[�v����ʂ��邪�A������Ȃ��Ȃ�܂ŌJ��Ԃ����[�v�͌p��
				if (isHit && !isFirstHit)
				{
					// �������Ă����ꍇ��velocity��1�h�b�g���k�߂Ă����A������Ȃ��Ȃ�܂ŌJ��Ԃ�
					// ���̂܂܏k�߂Ă��܂��ƁA�΂߂̃x�N�g���̂Ƃ��i�Ⴆ�ΕǂɌ������ăW�����v�����Ƃ��j�ɃW�����v�̐������k�߂Ă��܂�
					// �����h�����߂ɁA
					// ����������k�߂Ă������ƂŁA�����������
					float absX = fabsf(resultVel.x);	// velocity��x�����̐�Βl
					float absY = fabsf(resultVel.y);	// velocity��y�����̐�Βl

					// x�������k�ߐ؂��Ă��Ȃ����x�������k�߂�
					// x�������k�߂邩�ǂ���
					// false�Ȃ�k�ߐ؂��Ă���̂�y���k�߂�
					bool shrinkX = (absX != 0.0f);

					if (shrinkX)
					{
						if (resultVel.x > 0.0f) resultVel.x -= 1.0f;
						else                    resultVel.x += 1.0f;

						// �k�ߐ؂��������
						if (fabs(resultVel.x) < 1.0f) resultVel.x = 0.0f;

						loop = true;
					}
					else
					{
						if (resultVel.y > 0.0f) resultVel.y -= 1.0f;
						else                    resultVel.y += 1.0f;

						// �k�ߐ؂��������
						if (fabs(resultVel.y) < 1.0f) resultVel.y = 0.0f;

						loop = true;
					}

					break;
				}

				/* ��dfor�� */
			}
			if (isHit) break;
		}
	}

	return resultVel;
}

bool Map::CheckHitMapChip(const Vector2 pos, const Game::Size size, const int chipX, const int chipY) const
{
	std::shared_ptr<MapChip> chip = GetMapChipData(chipX, chipY);

	// ��O����
	if (chip == nullptr)
	{
		assert(false && "�͈͊O�ǂݎ��");
		return false;
	}

	// �����蔻��������Ȃ��}�b�v�`�b�v�Ȃ画�菈�����΂�
	int chipNo = chip->GetMapChipNum();
	if (Game::CheckEmptyMapChip(chipNo)) return false;

	Vector2 chipPos = chip->GetPos();
	Game::Size chipSize = chip->GetBoxSize();

	// �������Ă��邩�ǂ������ׂ�
	int posTop    = pos.y - (size.height * 0.5f);
	int posBottom = pos.y + (size.height * 0.5f);
	int posRight  = pos.x + (size.width * 0.5f);
	int posLeft   = pos.x - (size.width * 0.5f);
	int chipTop    = chipPos.y - (chipSize.height * 0.5f);
	int chipBottom = chipPos.y + (chipSize.height * 0.5f);
	int chipRight  = chipPos.x + (chipSize.width * 0.5f);
	int chipLeft   = chipPos.x - (chipSize.width * 0.5f);

	// ��`���m�̓����蔻��
	// ������Ȃ��p�^�[�����o���Ă���𔽓]�����Ă���
	bool isHit = !(
		(posTop >= chipBottom || posBottom <= chipTop) ||
		(posLeft >= chipRight || posRight <= chipLeft));
	if (isHit)
	{
		Vector2 offset = _camera.lock()->GetDrawOffset();
		
#ifdef _DEBUG

		DrawLine(pos.x + offset.x, pos.y + offset.y, chipPos.x + offset.x, chipPos.y + offset.y, 0xff0000);

#endif // _DEBUG

		return true;
	}
	else
	{
		return false;
	}
}
