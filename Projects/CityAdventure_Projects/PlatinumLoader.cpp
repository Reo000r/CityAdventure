#include "DxLib.h"
#include <vector>
#include "PlatinumLoader.h"
#include <string>
#include <cassert>

// �}�b�v�f�[�^�������Ă���ׂ̃N���X
// �����҂̃R�[�h���Q�l�ɂ��܂���

PlatinumLoader& PlatinumLoader::GetInstance()
{
	static PlatinumLoader instance;
	return instance;
}

void PlatinumLoader::Load(const TCHAR* filePath)
{
	// �t�@�C���̐擪�̏��Q��ۑ�����\����
	// FMF�w�b�_�[(Platinum�̃h�L�������g�ɏ����Ă���)
	struct Header {
		int8_t id[4];			//���ʎq(FMF_)			1*4�o�C�g
		uint32_t size;			//�f�[�^�T�C�Y�@		4�o�C�g
		uint32_t mapWidth;		//�}�b�v�̕�			4�o�C�g
		uint32_t mapHeight;		//�}�b�v�̍����@		4�o�C�g
		uint8_t chipWidth;		//�`�b�v(�Z�����)�̕�					1�o�C�g
		uint8_t chipHeight;		//�`�b�v(�Z�����)�̍���				1�o�C�g
		uint8_t layerCount;		//���C���[�̐�							1�o�C�g
		uint8_t bitCount;		//�P�Z��������̃r�b�g��(��8�Ńo�C�g��)	1�o�C�g
	};//20�o�C�g

	// �t�@�C���̐擪�̏��Q��ǂ�
	Header header;
	int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);

	// �t�@�C���̐擪�̕�����ǂ�
	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());

	// �擪��FMF_������������assert
	if (strId != "FMF_") {
		assert(0);
	}

	// �����o�ϐ��ɏ���ۑ�
	_mapWidth = header.mapWidth;
	_mapHeight = header.mapHeight;
	int layerCount = header.layerCount;
	//���C���[1������̃T�C�Y���v�Z����
	//�}�b�v�̕����}�b�v�̍���*(�`�b�v1������̃o�C�g��)
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	// 
	mapData_.resize(layerCount);
	for (auto& layer : mapData_) {
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);
}

const MapData_t& PlatinumLoader::GetMapData() const
{
	return mapData_;
}

const int PlatinumLoader::GetChipSpriteNo(LayerType layerType, int chipX, int chipY) const
{
	assert(chipX < _mapWidth);
	assert(chipY < _mapHeight);
	auto index = chipX + chipY * _mapWidth;
	return mapData_[(int)layerType][index];
}

void PlatinumLoader::GetMapSize(int& width, int& height)
{
	width = _mapWidth;
	height = _mapHeight;
}
