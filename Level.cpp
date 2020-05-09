#include "Level.h"
/*
Level::Level(int a): count_of_levels(2)
{
	count_of_enemy = a;
}
*/
void Level::Levels()
{
	for (int i = 0; i < count_of_levels; ++i)
	{
		Logic l(30, i, i, i);
		if (l.Run() == false)
			return;
	}
}