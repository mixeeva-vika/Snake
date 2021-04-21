#pragma once
#include"Field.h"
#include"Snake.h"
#include"Enemy.h"
#include "FreezingFood.h"
#include "FoodCanNotEatSnake.h"
#include "Food.h"

enum class GameState
{
	Continue,
	Win, 
	Losing
};

class Logic : public EventSubscriber
{
    //const char snake_symbol = 219;
    //const char food_symbol = '*';
	//const char enemy_symbol = 197;
	const int count_of_block;
	std::vector<Point> block;
	//const char block_symbol = 186;
	int count_of_enemy;
	int power_of_brean_of_enemy;
	std::vector<Point> offset_points = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };

	Field field;
	EventManager event_manager;
    Snake snake;
	Enemy enemy;
    Food food;
	FreezingFood freezingfood;
	FoodCanNotEatSnake food_can_not_eat_snake;
	GameState game_state;

	void GenerateMoveEvent(char c);
	void GenerateBlockPosition();
	void GenerateEnemyPosition();
	void InitializeTheGame();

	Point GenerateNeighborPoint(Point p, bool snake_intersections = true, Point exceptional_point = { 0, 0 });
	Point NewEnemyPosition(Point enemy_coordinates, Point smart_point);
	bool MoveEnemy(int idx, Point smart_point);
	bool MoveAllEnemy();

    void ThreadFunction1(char& new_dir);///////////////////////
    void ThreadFunction2(char& new_dir, bool& cond);/////////////////////////
	
public:
    Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_);
	~Logic();
    bool Run();

	void OnEvent(EventType);
    
};
