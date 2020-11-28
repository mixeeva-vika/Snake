#pragma once
#include<iostream>
#include"Point.h"
#include <ctime>
class FreezingFood
{
	const char freezing_food_symbol = 43;
	Point freezing_food = { 0, 0 };
	unsigned int start_time_or_disappearance_time = clock();
	time_t time_in_the_field = 9000;
	time_t time_away_from_the_field = 6000;
public:
	Point Get();
	void Set(const Point& p);
	bool NeedToGeneratePoint();
	bool NeedToClear();
	const char GetSymbol();
};