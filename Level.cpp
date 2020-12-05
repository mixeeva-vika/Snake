#include "Level.h"
#include "Field.h"
#include <thread>
#include <chrono>
/*
Level::Level(int a): count_of_levels(2)
{
	count_of_enemy = a;
}
*/
void Level::Levels()
{
	int count_of_lifes = 3;
	PrintHelper pr;
	Field field;
	for (int i = 0; i < count_of_levels; ++i)
	{
		bool success = false;
		int lifes = 0;
		Logic l(10 + i, i + 3, i, i + 5);
		for (int j = 0; j < count_of_lifes; ++j)
		{
			field.Clear();
			field.PrintLevel(i + 1, j + 1);
			std::this_thread::sleep_for(2s);
			if (l.Run())
			{
				success = true;
				break;
			}
		}
		if (!success)
		{
			field.PrintGameOver();
			std::this_thread::sleep_for(2s);
			return;
		}
	}
}