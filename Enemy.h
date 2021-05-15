#pragma once
#include<iostream>
#include<vector>
#include <ctime>
#include"EventSubscriber.h"
#include"Point.h"
#include"Field.h"
#include"EventManager.h"
#include"Snake.h"
 
class Enemy: public EventSubscriber
{
	std::vector<Point> offset_points = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };

	std::vector<Point> points;
	int count_of_enemy;
	int power_of_brean_of_enemy;
	bool freezing = false;
	bool can_be_eaten = false;

	bool snake_can_be_eaten = true;
	unsigned int start_time_for_snake_can_not_be_eaten = clock();
	time_t time_snake_can_not_be_eaten = 10000;

	unsigned int start_freezing_time = clock();
	time_t freezing_time = 5000;

	unsigned int start_stop_time = clock();
	time_t time_for_move = 1000;
	
	Field& field;
	EventManager& event_manager;

	void Init();
public: 
	Enemy(Field& field_, EventManager& event_manager_, int count_of_enemy_, int power_of_brean_of_enemy_);
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

	Point GenerateNeighborPoint(Point p, bool snake_intersections = true, Point exceptional_point = { 0, 0 });
	Point NewEnemyPosition(Point enemy_coordinates, Point smart_point);
	bool MoveEnemy(int enemy_idx, Point smart_point);
	bool MoveAllEnemy();
	bool SnakeGetCanBeEaten();
	void SnakeSetCanBeEaten();
	
	bool NeedToMove();
	void OnEvent(Event*);
	void Action();
};