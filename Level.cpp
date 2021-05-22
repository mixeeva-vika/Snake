#include "Level.h"
#include "Logic.h"
#include "Field.h"
#include "Point.h"

#include <thread>
#include <chrono>

void Level::PrintLevel(int level_number, int attempt_number, PrintHelper pr)
{
	std::string current_level = "Level " + std::to_string(level_number);
	std::string current_try = "Attempt " + std::to_string(attempt_number);
	pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 5 }, current_level);
	pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 3 }, current_try);
}

void Level::Levels()
{
	const int count_of_lifes = 3;
	PrintHelper pr;
	for (int i = 0; i < count_of_levels; ++i)
	{
		bool success = false;
		for (int j = 0; j < count_of_lifes; ++j)
		{
			PrintLevel(i + 1, j + 1, pr);
			std::this_thread::sleep_for(2s);

			Logic l(10 + i, i + 3, i, i + 5);
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
	
	pr.Print(Point{ 45 / 2 - 3, 25 / 2 - 5 }, "You are win");
	std::this_thread::sleep_for(2s);
	return;
}