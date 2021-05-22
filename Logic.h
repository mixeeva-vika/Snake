#pragma once
#include"Field.h"

enum class GameState
{
	Continue,
	Win, 
	Losing
};

class Logic : public EventSubscriber
{
	EventManager event_manager;
	Field field;

	std::vector<EventSubscriber*> game_objects;
	GameState game_state;
	
	void GenerateMoveEvent(char c);
    void GameAction(char& new_dir);
    void ReadKey(char& new_dir, bool& cond);
	
public:
    Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_);
	~Logic();
    bool Run();
	void OnEvent(Event*);  
};
