#include"Block.h"
#include <assert.h>

Block::Block(Field& field_, EventManager& event_manager_, int count_of_block_)
	: field(field_), 
    event_manager(event_manager_), 
    count_of_block(count_of_block_)
{
	event_manager.SubscribeOnEvent(this, InitializeTheGame);
}


void Block::Init()
{
	for (int i = 0; i < count_of_block; i++)
	{
		Point cur = field.GeneratePoint();
		points.push_back(cur);
		field.Set(cur, Objects::Block);
	}
}

void Block::OnEvent(Event* et)
{
	if (et->GetEventType() == EventType::InitializeTheGame)
	{
		Init();
		return;
	}
	throw;
}

