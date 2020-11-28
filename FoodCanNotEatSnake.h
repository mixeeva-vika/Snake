#pragma once
#include<iostream>
#include"Point.h"
#include <ctime>
class FoodCanNotEatSnake
{
	const char food_can_not_eat_snake_symbol = 3;
	Point food_can_not_eat_snake = { 0, 0 };
	unsigned int start_time_or_disappearance_time = clock();
	time_t time_in_the_field = 12000;
	time_t time_away_from_the_field = 9000;
public:
	Point Get();
	void Set(const Point& p);
	bool NeedToGeneratePoint();
	bool NeedToClear();
	const char GetSymbol();
};