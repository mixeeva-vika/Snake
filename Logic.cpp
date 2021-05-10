#include"Logic.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <assert.h>
#include<iostream>

Logic::Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_) :
	snake(field, event_manager),
	enemy(field, event_manager, count_of_enemy_, power_of_brean_of_enemy_),
	freezingfood(field, event_manager),
	food_can_not_eat_snake(field, event_manager),
	food(field, event_manager),
	block(field, event_manager, count_of_block_),
	field(event_manager)
{
	event_manager.SubscribeOnEvent(this, EventType::Win);
	event_manager.SubscribeOnEvent(this, EventType::Losing);

    srand(static_cast<unsigned int>(time(0)));
	game_state = GameState::Continue;
}

Logic::~Logic()
{
	field.Clear();
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




void Logic::ThreadFunction1(char& key)
{
	event_manager.PushEvent(EventType::InitializeTheGame);
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