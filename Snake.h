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
    void Move(Point p);
    void Add(Point p);
    bool PointBelongsToTheSnake(Point p);
    bool PointIsSecondElemOfSnake(Point p);
    int Size();
};