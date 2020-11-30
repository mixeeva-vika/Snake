#pragma once
#include<iostream>
#include"Point.h"
#include <ctime>
#include"Field.h"
class FreezingFood
{
	const char freezing_food_symbol = 43;
	Point freezing_food = { 0, 0 };
	unsigned int start_time_or_disappearance_time = clock();
	time_t time_in_the_field = 9000;
	time_t time_away_from_the_field = 6000;
	Field& field;
public:
	FreezingFood(Field& field_) : field(field_){}
	Point Get();
	void Set(const Point& p);
	bool NeedToGeneratePoint();
	bool NeedToClear();
	const char GetSymbol();
};