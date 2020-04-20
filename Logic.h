#pragma once
#include"Snake.h"
#include"Enemy.h"
#include"PrintHelper.h"


class Logic
{
    const short height = 25;
    const short width = 45;
    const char snake_symbol = 219;
    const char food_symbol = '*';
	const char enemy_symbol = 197;
    const Point snake_start_position{ 44, 10 };
    const Point game_over_point_position1{ width / 2 - 5, height / 2 };
    const string game_over1 = "Game Over";
    const Point game_over_point_position2{ width / 2 - 9, height /2 + 2};
    const string game_over2 = "Press Enter to exit";
    const int snake_size_for_win = 10;
    const Point win_point_position = { width / 2 - 15, height / 2 };
    const string win = "Congratulations! This game is yours";
	const int count_of_block = 10;
	vector<Point> block;
	const char block_symbol = 186;
	int count_of_enemy = 10;

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
	bool MoveEnemy(int idx);
	bool MoveAllEnemy();
    void ThreadFunction1(char& new_dir, bool& you_win);///////////////////////
    void ThreadFunction2(char& new_dir, bool& cond);/////////////////////////
    void Clear();
	void ClearTailOfSnake(Point p);
	int DistanceBetweenPoints(Point a, Point b);
	Point ShortestDirectionTowardsTheSnake(Point enemy);
public:
    Logic();
    void Run();
    
};
