#include "FreezingFood.h"

Point FreezingFood::Get()
{
	return freezing_food;
}
void FreezingFood::Set(const Point& p)
{
	start_time_or_disappearance_time = clock();
	freezing_food = p;
}

bool FreezingFood::NeedToGeneratePoint()
{
	unsigned int cur_time = clock();
	if ((freezing_food == Point{ 0,0 }) && ((cur_time - start_time_or_disappearance_time >= time_away_from_the_field)))
		return true;
	return false;
}

bool FreezingFood::NeedToClear()
{
	if (freezing_food != Point{ 0,0 })
	{
		unsigned int cur_time = clock();
		if (cur_time - start_time_or_disappearance_time >= time_in_the_field)
		{
			return true;
		}
	}
	return false;
}

const char FreezingFood::GetSymbol()
{
	return freezing_food_symbol;
}