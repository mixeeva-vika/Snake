#pragma once
#include"PrintHelper.h"

class Level
{
	int count_of_enemy;
	int count_of_blocks;
	int speed;
	int power_of_brean_of_enemy;
	const int count_of_levels = 10;

	void PrintLevel(int level_number, int attempt_number, PrintHelper pr);
public:
	void Levels();
};