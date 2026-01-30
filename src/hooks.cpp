#include "hooks.h"
#include "events_queue.h"

void OnStartFramePre()
{
	ConsumeQueueEvents();
}