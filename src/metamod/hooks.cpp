#include "hooks.h"
#include "mpsc/events_queue.h"

void OnStartFramePre()
{
	ConsumeQueueEvents();
}