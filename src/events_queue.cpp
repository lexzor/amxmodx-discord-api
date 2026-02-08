#include "events_queue.h"
#include "precompiled.h"

std::unique_ptr<Queue<EventFn>> g_EventsQueue = std::make_unique<Queue<EventFn>>();

void InitializeEventsQueue()
{
	g_EventsQueue->Initialize();
}

void ConsumeQueueEvents()
{
	std::shared_ptr<Queue<EventFn>::Node> node = nullptr;
	while ((node = g_EventsQueue->Pop()) != nullptr)
	{
		node->func();
	}
}