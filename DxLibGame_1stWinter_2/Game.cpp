#include "Game.h"

bool Game::CheckEmptyMapChip(int chipNum)
{
	if (chipNum == Game::kNotColChipNum1 ||
		chipNum == Game::kNotColChipNum2 ||
		chipNum == Game::kNotColChipNum3)
	{
		return true;
	}

	return false;
}
