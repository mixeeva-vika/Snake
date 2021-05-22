#include"Logic.h"
#include"Snake.h"
#include"Enemy.h"
#include"Block.h"
#include "Food.h"
#include "FreezingFood.h"
#include "FoodCanNotEatSnake.h"

#include <thread>
#include <chrono>
#include <assert.h>

Logic::Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_) :field(event_manager)
{
	game_objects.push_back(new Snake             (field, event_manager));
	game_objects.push_back(new Enemy             (field, event_manager, count_of_enemy_, power_of_brean_of_enemy_));
	game_objects.push_back(new Block             (field, event_manager, count_of_block_));
	game_objects.push_back(new Food              (field, event_manager));
	game_objects.push_back(new FreezingFood      (field, event_manager));
	game_objects.push_back(new FoodCanNotEatSnake(field, event_manager));

	event_manager.SubscribeOnEvent(this, EventType::Win);
	event_manager.SubscribeOnEvent(this, EventType::Losing);

    srand(static_cast<unsigned int>(time(0)));
	game_state = GameState::Continue;
}

Logic::~Logic()
{
	field.Clear();
	for (int i = 0; i < game_objects.size(); ++i)
	{
		delete game_objects[i];
	}
}

void Logic::GenerateMoveEvent(char c)
{
	if (c == 'a')
	{ 
		event_manager.PushEvent(new Event(MoveLeft));
	}
	else if (c == 'w')
	{
		event_manager.PushEvent(new Event(MoveUp));
	}
	else if (c == 'd')
	{
		event_manager.PushEvent(new Event(MoveRight));
	}
	else if (c == 's')
	{
		event_manager.PushEvent(new Event(MoveDown));
	}	
}

void Logic::GameAction(char& key)
{
	event_manager.PushEvent(new Event(InitializeTheGame));
	event_manager.EventHandler();
	char new_dir = 0;
	while (game_state == GameState::Continue)
	{
		if (key == 0)
		{
			continue;
		}
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
		
		for (int i = 0; i < game_objects.size(); ++i)
		{
			game_objects[i]->Action();
		}
		event_manager.EventHandler();
    }
}

void Logic::ReadKey(char& key, bool& cond)
{
    char c;
    while (cond)
    {
        c = _getch();
		if ((c == 'a') || (c == 'w') || (c == 'd') || (c == 's') || (c == 'n') || (c == 'i'))
		{
			key = c;
		}
		else
		{
			continue;
		}
    } 
}

 bool Logic::Run()
 {
	 bool cond = true;
	 char key = 0; // новое направление, в котором движется змея
	 std::thread thr1(&Logic::GameAction, this, std::ref(key));
	 std::thread thr2(&Logic::ReadKey, this, std::ref(key), std::ref(cond));
	 thr1.join();
	 cond = false;
	 if (game_state == GameState::Losing)
	 {
		 field.PrintLevelOver();
	 }
	 else
	 {
		 field.PrintWin();
	 }
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