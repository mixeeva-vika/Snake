#include"Logic.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <assert.h>
#include<iostream>

Logic::Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_) :
	field(event_manager),
	snake(field, event_manager),
	enemy(field, event_manager, count_of_enemy_, power_of_brean_of_enemy_),
	block(field, event_manager, count_of_block_),
	food(field, event_manager),
	freezingfood(field, event_manager),
	food_can_not_eat_snake(field, event_manager)
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
		event_manager.PushEvent(new Event(MoveLeft));
	}
	if (c == 'w')
	{
		event_manager.PushEvent(new Event(MoveUp));
	}
	if (c == 'd')
	{
		event_manager.PushEvent(new Event(MoveRight));
	}
	if (c == 's')
	{
		event_manager.PushEvent(new Event(MoveDown));
	}	

}




void Logic::ThreadFunction1(char& key)
{
	event_manager.PushEvent(new Event(InitializeTheGame));
	event_manager.EventHandler();
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
		if (new_dir == 0)
		{
			continue;
		}

        GenerateMoveEvent(new_dir);
        std::this_thread::sleep_for(200ms);
		snake.Action();
		food.Action();
		freezingfood.Action();
		food_can_not_eat_snake.Action();
		enemy.Action();
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
	 
	 field.Clear();
	 return game_state == GameState::Win;
 }

 void Logic::OnEvent(Event* et)
 {
	 if (et->GetEventType() == EventType::Win)
	 {
		 game_state = GameState::Win;
	 }
	 if (et->GetEventType() == EventType::Losing)
	 {
		 game_state = GameState::Losing;
	 }
 }