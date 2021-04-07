
#include "EventManager.h"

void  EventManager::EventHandler()
{
	while (!events.empty())
	{
		EventType et = events.front();
		auto& subscribers = event_subscriber[et];
		for (auto sub : subscribers)
		{
			sub->OnEvent(et);
		}
		events.pop();
	}
}