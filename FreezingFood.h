#pragma once
#include<iostream>
#include"EventSubscriber.h"
#include"Point.h"
#include <ctime>
#include"Field.h"
#include"EventManager.h"

class FreezingFood : public EventSubscriber
{
	const char freezing_food_symbol = 43;
	Point freezing_food = { 0, 0 };
	unsigned int start_time_or_disappearance_time = clock();
	time_t time_in_the_field = 9000;
	time_t time_away_from_the_field = 6000;
	Field& field;
	EventManager& event_manager;

	bool NeedToGeneratePoint();
	bool NeedToClear();

	Point Get();
	void Set(const Point& p);
	const char GetSymbol();

public:
	FreezingFood(Field& field_, EventManager& event_manager_);
	void Action();
	void OnEvent(EventType);
};