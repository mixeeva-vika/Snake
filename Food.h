#pragma once
#include<iostream>
#include"EventSubscriber.h"
#include"Point.h"
#include"Field.h"
#include"EventManager.h"

class Food : public EventSubscriber
{
	Point food = {0, 0};
	Field& field;
	EventManager& event_manager;

	Point Get();
	void Set(const Point& p);

public:
	Food(Field& field_, EventManager& event_manager_);
	void Action();
	void OnEvent(EventType);
};