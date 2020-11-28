#pragma once
#include<iostream>
#include <string>
#include <vector>
#include "Point.h"
#include "PrintHelper.h"

enum class Objects
{
	Empty,
	Snake,
	Enemy,
	Block,
	Food,
	FoodCanNotEatSnake,
	FoodFreezing
};

class Field
{
	const short height = 25;
	const short width = 45;
	const Point level_over_point_position1{ width / 2 - 5, height / 2 };
	const Point game_over_point_position2{ width / 2 - 9, height / 2 + 2 };
	const std::string level_over1 = "Level Is Over";
	const std::string press_enter = "Press Enter to continue";
	const Point win_point_position = { width / 2 - 15, height / 2 };
	const std::string win = "Congratulations! This level is yours";
	//const std::vector<unsigned char> symbols = {' ', '█', '┼', '║', '*', '♥', '+'};
	//const std::vector<unsigned char> symbols = { 32, 135, '┼', 89, '*', 1, '+' };
	const std::vector<unsigned char> symbols = { 32, 127, 12, 89, 5, 1, 19 };

	PrintHelper pr;
	std::vector<std::vector<Objects>> field;

public:
	Field();
	void Clear();
	void DrawTheFieldBoundary();
	Point GeneratePoint(const std::vector<Objects>& v = std::vector<Objects>());//передаем список тех, на кого можно наступать
	void Set(Point p, Objects ob);
	Objects Get(Point p);
	bool InTheField(Point p);
	std::pair<short, short> GetFieldSize();
	void PrintLevelOver();
	void PrintWin();
	void PrintLevel(int level_number, int attempt_number);
	void PrintGameOver();
 };