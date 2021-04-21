#include"Logic.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <assert.h>
#include<iostream>

Logic::Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_):
	snake(field, event_manager),
    count_of_enemy(count_of_enemy_), 
	count_of_block(count_of_block_),
	power_of_brean_of_enemy(power_of_brean_of_enemy_),
	enemy(count_of_enemy, field, event_manager),
	freezingfood(field, event_manager),
	food_can_not_eat_snake(field, event_manager),
	food(field, event_manager)
{
	event_manager.SubscribeOnEvent(this, EventType::Win);
	event_manager.SubscribeOnEvent(this, EventType::Losing);

    srand(static_cast<unsigned int>(time(0)));
	block.resize(count_of_block); 
	game_state = GameState::Continue;
}

Logic::~Logic()
{
	field.Clear();
}

void Logic::GenerateBlockPosition()
{
	for (int i = 0; i < count_of_block; ++i)
	{
		block[i] = field.GeneratePoint();
		field.Set(block[i], Objects::Block);
	}
}

void Logic::GenerateEnemyPosition()
{
	for (int i = 0; i < count_of_enemy; ++i)
	{
		Point new_pos;
		//do
		//{
			new_pos = field.GeneratePoint();
		//} while (new_pos.Distance(snake.Head()) < 5);
		for (int j = 0; j < i; ++j)
		{
			assert(new_pos != enemy.Get(j));
		}
		enemy.Set(new_pos, i);
	}
}

void Logic::InitializeTheGame()
{
	field.Clear();
	field.DrawTheFieldBoundary();
	GenerateBlockPosition();
	GenerateEnemyPosition();
	snake.Action();
}

void Logic::GenerateMoveEvent(char c)
{
	if (c == 'a')
	{ 
		event_manager.PushEvent(EventType::MoveLeft);
	}
	if (c == 'w')
	{
		event_manager.PushEvent(EventType::MoveUp);
	}
	if (c == 'd')
	{
		event_manager.PushEvent(EventType::MoveRight);
	}
	if (c == 's')
	{
		event_manager.PushEvent(EventType::MoveDown);
	}	

}

Point Logic::GenerateNeighborPoint(Point p, bool snake_intersections, Point exceptional_point)
{
	int new_direction = rand() % 4;

	for (int i = 0; i < offset_points.size(); ++i)
	{
		Point new_point = p + offset_points[new_direction];
		if(field.InTheField(new_point) && (new_point != exceptional_point) &&
			((field.Get(new_point) == Objects::Empty) || (snake_intersections && field.Get(new_point) == Objects::Snake)))
			return new_point;
		new_direction = (new_direction + 1) % 4;
	}
	return p;
}

Point Logic::NewEnemyPosition(Point enemy_coordinates, Point smart_point)
{
	bool smart_point_is_correct = ((smart_point != Point{ 0,0 }) && (field.Get(smart_point) != Objects::Enemy) && (snake.GetCanBeEaten() || field.Get(smart_point) != Objects::Snake));
	int step = rand() % 10;
	if (smart_point_is_correct && (step <= power_of_brean_of_enemy))
		return smart_point;
	return GenerateNeighborPoint(enemy_coordinates, snake.GetCanBeEaten());
}

bool Logic::MoveEnemy(int enemy_idx, Point smart_point) 
{
	Point enemy_coordinates = enemy.Get(enemy_idx);
	Point new_enemy_coordinates = NewEnemyPosition(enemy_coordinates, smart_point);
	assert(snake.GetCanBeEaten() || field.Get(new_enemy_coordinates) != Objects::Snake);
	if (snake.PointBelongsToTheSnake(new_enemy_coordinates))
	{
		if (new_enemy_coordinates == snake.Head())
			return false;
		else
			snake.CutOfTail(new_enemy_coordinates);
	}
	enemy.Set(new_enemy_coordinates, enemy_idx);
	return true;
}

bool Logic::MoveAllEnemy()
{
	std::vector<Point> smart_position = enemy.ShortestDirectionTowardsTheSnake();
	for (int i = 0; i < enemy.Size(); ++i)
	{
		if (MoveEnemy(i, smart_position[i]) == false)
			return false;
	}
	return true;
}

void Logic::ThreadFunction1(char& key)
{
	InitializeTheGame();
    Point new_pos;
	int count = 1;
	char new_dir = 0;
	ofstream file("out.txt");
    while (game_state == GameState::Continue)
    {
		if (key == 0)
			continue;
		if (key == 'n')
		{
			game_state = GameState::Win;
			return;
		}
		/*
		if (key == 'i')
		{
			Point tail = snake.Tail();
			Point new_tail = GenerateNeighborPoint(tail, false, MovePoint(snake.Head(), new_dir));
			if(tail != new_tail)
				snake.AddTail(new_tail);
			field.Set(new_tail, Objects::Snake);
			if (snake.Size() == snake_size_for_win)
			{
				game_state = GameState::Win;
				return;
			}
			key = new_dir;
		}
		*/
		if ((key != 'n') && (key != 'i'))
		{
			new_dir = key;
		}
        GenerateMoveEvent(new_dir);
        std::this_thread::sleep_for(200ms);
		snake.Action();
		food.Action();
		freezingfood.Action();
		food_can_not_eat_snake.Action();
		if ((count % 3 == 0) && (new_dir != 0) && (!enemy.GetFreezing()) && (MoveAllEnemy() == false))
		{
			return;
		}
		++count;
		event_manager.EventHandler();
    }
}

void Logic::ThreadFunction2(char& key, bool& cond)
{
    char c;
    while (cond)
    {
        c = _getch();
        if ((c == 'a') || (c == 'w') || (c == 'd') || (c == 's') || (c == 'n') || (c == 'i'))
            key = c;
        else
            continue;
    } 
}

 bool Logic::Run()
 {
	 bool cond = true;
	 //bool you_win = false;
	 char key = 0; // новое направление, в котором движется змея
	 std::thread thr1(&Logic::ThreadFunction1, this, std::ref(key));
	 std::thread thr2(&Logic::ThreadFunction2, this, std::ref(key), std::ref(cond));
	 thr1.join();
	 cond = false;
	 if (game_state == GameState::Losing)
		 field.PrintLevelOver();
	 else
		 field.PrintWin();
	 thr2.join();
	 return game_state == GameState::Win;
 }

 void Logic::OnEvent(EventType et)
 {
	 if (et == EventType::Win)
	 {
		 game_state = GameState::Win;
	 }
	 if (et == EventType::Losing)
	 {
		 game_state = GameState::Losing;
	 }
 }