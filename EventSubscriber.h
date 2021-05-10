#pragma once
#include "Point.h"
enum EventType
{
	InitializeTheGame,
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

class Event
{
	EventType  event_type;
public:
	Event(EventType et): event_type(et){}
	EventType GetEventType()  {return event_type; }
};

class EventWithPoint : public Event
{
	Point p;
public:
	EventWithPoint(EventType et, Point p_):Event(et), p(p_){}
	Point GetPoint() { return p; }

};

class EventSubscriber
{
public:
	virtual void OnEvent(EventType) = 0;
};

