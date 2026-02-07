#pragma once

#include <functional>
#include <memory>
#include <concepts>
#include <dpp/dispatcher.h>
#include "mpsc/queue.h"

using EventFn = std::function<void()>;

extern std::unique_ptr<Queue<EventFn>> g_EventsQueue;

void InitializeEventsQueue();
void ConsumeQueueEvents();