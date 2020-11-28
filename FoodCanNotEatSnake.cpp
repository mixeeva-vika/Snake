#include "FoodCanNotEatSnake.h"

Point FoodCanNotEatSnake::Get()
{
	return food_can_not_eat_snake;
}
void FoodCanNotEatSnake::Set(const Point& p)
{
	start_time_or_disappearance_time = clock();
	food_can_not_eat_snake = p;
}

bool FoodCanNotEatSnake::NeedToGeneratePoint()
{
	unsigned int cur_time = clock();
	if ((food_can_not_eat_snake == Point{ 0,0 }) && ((cur_time - start_time_or_disappearance_time >= time_away_from_the_field)))
		return true;
	return false;
}

bool FoodCanNotEatSnake::NeedToClear()
{
	if (!(food_can_not_eat_snake == Point{ 0,0 }))
	{
		unsigned int cur_time = clock();
		if (cur_time - start_time_or_disappearance_time >= time_in_the_field)
		{
			return true;
		}
	}
	return false;
}

const char FoodCanNotEatSnake::GetSymbol()
{
	return food_can_not_eat_snake_symbol;
}