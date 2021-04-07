#include "FreezingFood.h"

FreezingFood::FreezingFood(Field& field_, EventManager& event_manager_) : field(field_), event_manager(event_manager_)
{
	event_manager.SubscribeOnEvent(this, SnakeEatFoodFreezing);
}

Point FreezingFood::Get()
{
	return freezing_food;
}
void FreezingFood::Set(const Point& p)
{
	start_time_or_disappearance_time = clock();
	if (p != Point{ 0, 0 })
	{
		field.Set(p, Objects::FoodFreezing);
	}
	else
	{
		field.Set(freezing_food, Objects::Empty);
	}
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

void FreezingFood::Action()
{
	if (NeedToGeneratePoint())
	{
		Set(field.GeneratePoint());
	}
	else if (NeedToClear())
	{
		Set(Point{ 0,0 });
	}
}

void FreezingFood::OnEvent(EventType et)
{
	if (et == EventType::SnakeEatFoodFreezing)
	{
		Set(Point{ 0,0 });
		return;
	}
	throw;
}