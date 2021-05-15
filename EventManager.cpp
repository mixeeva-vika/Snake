
#include "EventManager.h"

void  EventManager::EventHandler()
{
	while (!events.empty())
	{
		EventType et = events.front()->GetEventType();
		auto& subscribers = event_subscriber[et];
		for (auto sub : subscribers)
		{
			sub->OnEvent(events.front());
		}
		delete events.front();
		events.pop();
	}
}