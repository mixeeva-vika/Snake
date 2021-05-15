#pragma once
#include"Field.h"
#include"Snake.h"
#include"Block.h"
#include"Enemy.h"
#include "FreezingFood.h"
#include "FoodCanNotEatSnake.h"
#include "Food.h"

enum class GameState
{
	Continue,
	Win, 
	Losing
};

class Logic : public EventSubscriber
{
    //const char snake_symbol = 219;
    //const char food_symbol = '*';
	//const char enemy_symbol = 197;
	//const char block_symbol = 186;

	EventManager event_manager;
	Field field;
    Snake snake;
	Enemy enemy;
	Block block;
    Food food;
	FreezingFood freezingfood;
	FoodCanNotEatSnake food_can_not_eat_snake;

	GameState game_state;

	void GenerateMoveEvent(char c);

    void ThreadFunction1(char& new_dir);
    void ThreadFunction2(char& new_dir, bool& cond);
	
public:
    Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_);
	~Logic();
    bool Run();
	void OnEvent(Event*);  
};
