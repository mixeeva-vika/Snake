#include "Food.h"

Food::Food(Field& field_, EventManager& event_manager_) : field(field_), event_manager(event_manager_)
{
	event_manager.SubscribeOnEvent(this, SnakeEatFood);
}

Point Food::Get()
{
	return food;
}
void Food::Set(const Point& p)
{
	if (p != Point{ 0, 0 })
	{
		field.Set(p, Objects::Food);
	}
	else
	{
		field.Set(food, Objects::Empty);
	}
	food = p;
}


void Food::Action()
{
	if (food == Point{ 0, 0 })
	{
		Set(field.GeneratePoint());
	}
	return;
}

void Food::OnEvent(EventType et)
{
	if (et == EventType::SnakeEatFood)
	{
		Set(field.GeneratePoint());
		return;
	}
	throw;
}