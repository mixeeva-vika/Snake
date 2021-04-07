#include "FoodCanNotEatSnake.h"

FoodCanNotEatSnake::FoodCanNotEatSnake(Field& field_, EventManager& event_manager_) : field(field_), event_manager(event_manager_)
{
	event_manager.SubscribeOnEvent(this, SnakeEatFoodCanNotEatSnake);
}

Point FoodCanNotEatSnake::Get()
{
	return food_can_not_eat_snake;
}
void FoodCanNotEatSnake::Set(const Point& p)
{
	start_time_or_disappearance_time = clock();
	if (p != Point{ 0, 0 })
	{
		field.Set(p, Objects::FoodCanNotEatSnake);
	}
	else
	{
		field.Set(food_can_not_eat_snake, Objects::Empty);
	}
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

void FoodCanNotEatSnake::Action()
{
	if (NeedToGeneratePoint())
	{
		//food_can_not_eat_snake.Set(field.GeneratePoint());
		Set(Point{ 44, 10 });
		field.Set(food_can_not_eat_snake, Objects::FoodCanNotEatSnake);
	}
	else if (NeedToClear())
	{
		field.Set(food_can_not_eat_snake, Objects::Empty);
		Set(Point{ 0,0 });
	}
}

void FoodCanNotEatSnake::OnEvent(EventType et)
{
	if (et == EventType::SnakeEatFoodCanNotEatSnake)
	{
		Set(Point{ 0,0 });
		return;
	}
	throw;
}