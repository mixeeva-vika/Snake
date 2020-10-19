#pragma once
#include<iostream>
#include<vector>
#include <ctime>
#include"Point.h"
 
class Enemy
{
	std::vector<Point> points;
	double brean;
	bool freezing = false;
	bool can_eat = false;
	unsigned int start_freezing_time = clock();
	time_t freezing_time = 5000;
public: 
	Enemy(int size);
	bool PointBelongsToTheEnemy(Point p);
	Point Get(int idx);
	int Size();
	void Set(Point p, int idx);
	const std::vector<Point>& GetPoints();
	bool GetFreezing();
	void SetFreezing();
	bool GetCanEat();
	void SetCanEat(bool eat);
};