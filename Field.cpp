#include "Field.h"
#include <assert.h>
Field::Field()
{
	field.resize(height);
	for (int i = 0; i < height; ++i)
		field[i].resize(width, Objects::Empty);
}

void Field::Clear()
{
	for (short i = 1; i < height; ++i)
	{
		for (short j = 1; j < width; ++j)
		{
			pr.Clear({ j, i });
			field[i][j] = Objects::Empty;
		}
	}
}

void Field::DrawTheFieldBoundary()
{
	for (short i = 0; i < width; ++i)
		pr.Print(Point{ i, 0 }, '-');
	for (short i = 0; i < width; ++i)
		pr.Print(Point{ i, height }, '-');
	for (short i = 0; i < height; ++i)
		pr.Print(Point{ 0, i }, '|');
	for (short i = 0; i < height; ++i)
		pr.Print(Point{ width , i }, '|');
}

Point Field::GeneratePoint(const std::vector<Objects>& v)
{
	Point p;
	while (true)
	{
		p.x = rand() % (width - 1) + 1;
		p.y = rand() % (height - 1) + 1;
		if ((Get(p) == Objects::Empty) || (find(v.begin(), v.end(), Get(p)) != v.end()))
		{
			return p;
		}
	}
}
 
void Field::Set(Point p, Objects ob)
{
	if (InTheField(p))
	{
		int a = 0;
	}
	field[p.y][p.x] = ob;
	pr.Print(p, symbols[static_cast<int>(ob)]);
}

Objects Field::Get(Point p)
{
	//assert(InTheField(p));
	if (InTheField(p) != true)
	{
		int a = 0;
	}
	return field[p.y][p.x];
}

bool Field::InTheField(Point p)
{
	return ((p.y > 0) && (p.y < height) && (p.x > 0) && (p.x < width));
}

std::pair<short, short> Field::GetFieldSize()
{
	return make_pair(width, height);
}

void Field::PrintLevelOver()
{
	Clear();
	pr.Print(level_over_point_position1, level_over1);
	pr.Print(game_over_point_position2, press_enter);
}

void Field::PrintWin()
{
	Clear();
	pr.Print(win_point_position, win);
	pr.Print(game_over_point_position2, press_enter);
}

void Field::PrintLevel(int level_number, int attempt_number)
{
	std::string current_level = "Level " + std::to_string(level_number);
	std::string current_try = "Attempt " + std::to_string(attempt_number);
	pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 5 }, current_level);
	pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 3 }, current_try);
}

void Field::PrintGameOver()
{
	pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 5 }, "Game Is Over");
}