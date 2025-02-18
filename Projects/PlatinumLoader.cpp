#include "DxLib.h"
#include <vector>
#include "PlatinumLoader.h"
#include <string>
#include <cassert>

// マップデータを持ってくる為のクラス
// ※先駆者のコードを参考にしました

PlatinumLoader& PlatinumLoader::GetInstance()
{
	static PlatinumLoader instance;
	return instance;
}

void PlatinumLoader::Load(const TCHAR* filePath)
{
	// ファイルの先頭の情報群を保存する構造体
	// FMFヘッダー(Platinumのドキュメントに書いてある)
	struct Header {
		int8_t id[4];			//識別子(FMF_)			1*4バイト
		uint32_t size;			//データサイズ　		4バイト
		uint32_t mapWidth;		//マップの幅			4バイト
		uint32_t mapHeight;		//マップの高さ　		4バイト
		uint8_t chipWidth;		//チップ(セル一個)の幅					1バイト
		uint8_t chipHeight;		//チップ(セル一個)の高さ				1バイト
		uint8_t layerCount;		//レイヤーの数							1バイト
		uint8_t bitCount;		//１セル当たりのビット数(÷8でバイト数)	1バイト
	};//20バイト

	// ファイルの先頭の情報群を読む
	Header header;
	int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);

	// ファイルの先頭の文字を読む
	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());

	// 先頭にFMF_が無かったらassert
	if (strId != "FMF_") {
		assert(0);
	}

	// メンバ変数に情報を保存
	_mapWidth = header.mapWidth;
	_mapHeight = header.mapHeight;
	int layerCount = header.layerCount;
	//レイヤー1個当たりのサイズを計算する
	//マップの幅＊マップの高さ*(チップ1個当たりのバイト数)
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
