#pragma once
#include<iostream>
#include<vector>
#include"Point.h"

class Snake
{
    std::vector<Point> points;
public:
    Snake(Point p);
    Point Head();
    Point Tail();
	bool CheckProximityOfPoints(Point p1, Point p2);
    void Move(Point p);
    void Add(Point p);
	void AddTail(Point p);
	void Cut(Point p);
    bool PointBelongsToTheSnake(Point p);
    bool PointIsSecondElemOfSnake(Point p);
    int Size();
	std::vector<Point> CutOfTail(Point p);
	const std::vector<Point>& GetPoints();
};