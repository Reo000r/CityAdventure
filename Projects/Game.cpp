#include "Game.h"

bool Game::CheckEmptyMapChip(int chipNum)
{
	if (chipNum == Game::kNotColChipNum1 ||
		chipNum == Game::kNotColChipNum2 ||
		chipNum == Game::kNotColChipNum3 ||
		chipNum == Game::kNotColChipNum4 ||
		chipNum == Game::kNotColChipNum5)
	{
		return true;
	}

	return false;
}
