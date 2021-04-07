#pragma once
#include<iostream>
#include<vector>
#include"EventSubscriber.h"
#include"Point.h"
#include <ctime>
#include"Field.h"
#include"EventManager.h"
class Snake : public EventSubscriber
{
    std::vector<Point> points;
	bool freezing = false;
	bool can_be_eaten = true;
	unsigned int start_time = clock();
	time_t time_can_not_be_eaten = 10000;
	Field& field;
	EventManager& event_manager;


public:
    Snake(Point p, Field& field_, EventManager& event_manager_);
    Point Head();
    Point Tail();
	bool CheckProximityOfPoints(Point p1, Point p2);
    void Move(Point p);
    void Add(Point p);
	void AddTail(Point p);
    bool PointBelongsToTheSnake(Point p);
    bool PointIsSecondElemOfSnake(Point p);
    int Size();
	std::vector<Point> CutOfTail(Point p);
	const std::vector<Point>& GetPoints();

	bool GetFreezing();
	void SetFreezing(bool freez);
	bool GetCanBeEaten();
	void SetCanBeEaten();

	void OnEvent(EventType);

};