#pragma once
#include<iostream>
#include<vector>
#include <ctime>
#include"EventSubscriber.h"
#include"Point.h"
#include"Field.h"
#include"EventManager.h"
 
class Enemy: public EventSubscriber
{
	std::vector<Point> points;
	double brean;
	bool freezing = false;
	bool can_be_eaten = false;
	unsigned int start_freezing_time = clock();
	time_t freezing_time = 5000;
	Field& field;
	EventManager& event_manager;
public: 
	Enemy(int size, Field& field_, EventManager& event_manager_);
	bool PointBelongsToTheEnemy(Point p);
	Point Get(int idx);
	int Size();
	void Set(Point p, int idx);
	const std::vector<Point>& GetPoints();
	bool GetFreezing();
	void SetFreezing();
	bool GetCanEat();
	void SetCanEat(bool can_be_eaten_);
	std::vector<Point> ShortestDirectionTowardsTheSnake();
	void OnEvent(EventType);
};