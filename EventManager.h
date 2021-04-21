#pragma once
#include <queue>
#include"EventSubscriber.h"

class EventManager
{
	std::queue<EventType> events;
	std::vector<std::vector<EventSubscriber*>> event_subscriber;
public:
	EventManager() { event_subscriber.resize(EventType::EventCount); }
	void PushEvent(EventType ev) { events.push(ev); }
	void SubscribeOnEvent(EventSubscriber* subscriber, EventType event) { event_subscriber[event].push_back(subscriber); }
	void EventHandler();
};

