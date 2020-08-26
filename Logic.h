#pragma once
#include"Snake.h"
#include"Enemy.h"
#include"PrintHelper.h"


class Logic
{
    const short height = 25;
    const short width = 45;
	//const short height = 7;
	//const short width = 7;
	const Point game_over_point_position1{ width / 2 - 5, height / 2 };
	const Point game_over_point_position2{ width / 2 - 9, height / 2 + 2 };
	const string game_over1 = "Game Over";
	const string game_ = "Press Enter to exit";
	const Point win_point_position = { width / 2 - 15, height / 2 };
	const string win = "Congratulations! This level is yours";

    const char snake_symbol = 219;
    const char food_symbol = '*';
	const char enemy_symbol = 197;
    const Point snake_start_position{ 44, 10 };
	//const Point snake_start_position{ 3, 3 };
    const int snake_size_for_win = 3;
	const int count_of_block;
	vector<Point> block;
	const char block_symbol = 186;
	int count_of_enemy;
	int power_of_brean_of_enemy;
	std::vector<Point> offset_points = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };

    Snake snake;
	Enemy enemy;
    Point food;
    PrintHelper pr;


    Point Change(Point p, char c);//////////////////////////////////////поменять названия
	Point GeneratePoint();
    Point GenerateFoodPosition();
	void GenerateBlockPosition();
	void GenerateEnemyPosition();///
	bool PointBelongsToTheBlock(Point p);
    void DrawTheField();
    char GenerateNewDirection();
	Point NewEnemyPosition(Point enemy_coordinates, Point smart_point);
	bool MoveEnemy(int idx, Point smart_point);
	bool MoveAllEnemy();
    void ThreadFunction1(char& new_dir, bool& you_win);///////////////////////
    void ThreadFunction2(char& new_dir, bool& cond);/////////////////////////
    void Clear();
	void ClearTailOfSnake(Point p);
	int DistanceBetweenPoints(Point a, Point b);
	Point ShortestDirectionTowardsTheSnake(Point enemy);
	bool PointInsideTheField(Point p);
	std::vector<Point> Logic::ShortestDirectionTowardsTheSnake();
public:
    Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_);
	~Logic();
    bool Run();
    
};
