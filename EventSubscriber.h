#pragma once
enum EventType
{
	SnakeEatFood,
	SnakeEatFoodFreezing,
	SnakeEatFoodCanNotEatSnake,
	EnemyCrossWithSnake,
	MoveUp,
	MoveLeft,
	MoveRight,
	MoveDown,
	Win,
	Losing,
	EventCount

};

class EventSubscriber
{
public:
	virtual void OnEvent(EventType) = 0;
};

