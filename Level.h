#pragma once
#include<iostream>
#include"Point.h"
#include"PrintHelper.h"
#include"Logic.h"

class Level
{
	int count_of_enemy;
	int count_of_blocks;
	int speed;
	int power_of_brean_of_enemy;
	const int count_of_levels = 10;
public:
	//Level(int a);
	void PrintLevel(int level_number, int attempt_number, PrintHelper pr);
	void Levels();
};