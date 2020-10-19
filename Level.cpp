#include "Level.h"
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
	for (int i = 0; i < count_of_levels; ++i)
	{
		bool success = false;
		int lifes = 0;
		Logic l(10 + i, i + 3, i, i);
		for (int j = 0; j < count_of_lifes; ++j)
		{
			std::string current_level = "Level " + std::to_string(i + 1);
			std::string current_try = "Attempt " + std::to_string(j + 1);
			pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 5 }, current_level);
			pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 3 }, current_try);
			std::this_thread::sleep_for(2s);
			if (l.Run())
			{
				success = true;
				break;
			}
		}
		if (!success)
		{
			pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 5 }, "Game Is Over");
			std::this_thread::sleep_for(2s);
			return;
		}
	}
}