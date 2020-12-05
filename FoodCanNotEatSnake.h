#pragma once
#include<iostream>
#include"Point.h"
#include <ctime>
#include "Field.h"
class FoodCanNotEatSnake
{
	const char food_can_not_eat_snake_symbol = 3;
	Point food_can_not_eat_snake = { 0, 0 };
	unsigned int start_time_or_disappearance_time = clock();
	time_t time_in_the_field = 8000;
	time_t time_away_from_the_field = 15000;
	Field& field;
public:
	FoodCanNotEatSnake(Field& field_) : field(field_) {}
	Point Get();
	void Set(const Point& p);
	bool NeedToGeneratePoint();
	bool NeedToClear();
	const char GetSymbol();
	void Action();
};