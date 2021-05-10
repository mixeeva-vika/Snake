#pragma once
#include<iostream>
#include<vector>
#include"EventSubscriber.h"
#include"Point.h"
#include <ctime>
#include"Field.h"
#include"EventManager.h"
class Block : public EventSubscriber
{
	std::vector<Point> points;
	Field& field;
	EventManager& event_manager;
	int count_of_block;

public:
	Block(Field& field_, EventManager& event_manager_, int count_of_block_);
	
	void Init();
	void OnEvent(EventType);
	void Action() {}

};