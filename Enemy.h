#pragma once
#include<iostream>
#include<vector>
#include <ctime>
#include"Point.h"
#include"Field.h"
 
class Enemy
{
	std::vector<Point> points;
	double brean;
	bool freezing = false;
	bool can_be_eaten = false;
	unsigned int start_freezing_time = clock();
	time_t freezing_time = 5000;
	Field& field;
public: 
	Enemy(int size, Field& field_);
	bool PointBelongsToTheEnemy(Point p);
	Point Get(int idx);
	int Size();
	void Set(Point p, int idx);
	const std::vector<Point>& GetPoints();
	bool GetFreezing();
	void SetFreezing();
	bool GetCanEat();
	void SetCanEat(bool can_be_eaten_);
};