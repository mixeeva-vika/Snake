#pragma once
enum EventType
{
	SnakeEatFood,
	SnakeEatFoodFreezing,
	SnakeEatFoodCanNotEatSnake,
	EnemyCrossWithSnake,
	Win,
	Losing


};
const int event_count = 4;

class EventSubscriber
{
public:
	virtual void OnEvent(EventType) = 0;
};

