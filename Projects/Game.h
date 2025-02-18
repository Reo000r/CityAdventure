#pragma once

//#define LOAD_DEBUG_STAGE
#define RELOAD_GAME_STAGE

namespace Game
{
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	constexpr float kScreenCenterWidth = kScreenWidth * 0.5f;
	constexpr float kScreenCenterHeight = kScreenHeight * 0.5f;

	constexpr int kFadeInterval = 30;

	constexpr int kSoundVolume = 128;

	// 何も描画しないマップ番号
	constexpr int kEmptyChipNum = 51;

	constexpr int kNotColChipNum1 = 51;  // 描画なし
	constexpr int kNotColChipNum2 = 10;  // 背景
	constexpr int kNotColChipNum3 = 39;  // 奥の箱
	constexpr int kNotColChipNum4 = 12;  // 背景との繋ぎの壁
	constexpr int kNotColChipNum5 = 14;  // 背景との繋ぎの壁



	// 矩形の縦横情報を保存する構造体
	struct Size
	{
		int width;
		int height;
	};

	// 位置を加味した矩形の情報を保存する構造体
	// メンバ変数として持たせる事は想定していない
	// GetRect()等を作りそこで取得し使う
	struct Rect
	{
		// 上
		int top;
		// 下
		int bottom;
		// 右
		int right;
		// 左
		int left;
	};

	/// <summary>
	/// 判定を残すマップチップ　だった　場合はfalse
	/// 　　　　　　　　　　　でなかった場合はtrue
	/// 今は3種類と判定
	/// </summary>
	bool CheckEmptyMapChip(int chipNum);
	
}

