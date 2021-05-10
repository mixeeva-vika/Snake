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
	const int snake_size_for_win = 3;
	Field& field;
	EventManager& event_manager;

	Point Tail();
	bool CheckProximityOfPoints(Point p1, Point p2);
	void Move(Point p);
	void Add(Point p);
	void AddTail(Point p);
	bool PointIsSecondElemOfSnake(Point p);
	int Size();
	bool GetFreezing();
	void SetFreezing(bool freez);
	const std::vector<Point>& GetPoints();
	void Init();

public:
    Snake(Field& field_, EventManager& event_manager_);
    Point Head();
   
    bool PointBelongsToTheSnake(Point p);
	void CutOfTail(Point p);

	void OnEvent(EventType);
	void Action();

};