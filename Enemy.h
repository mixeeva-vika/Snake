#pragma once
#include<iostream>
#include<vector>
#include"Point.h"
 
class Enemy
{
	std::vector<Point> points;
public: 
	Enemy(int size);
	bool PointBelongsToTheEnemy(Point p);
	Point Get(int idx);
	int Size();
	void Set(Point p, int idx);
	const std::vector<Point>& GetPoints();
};