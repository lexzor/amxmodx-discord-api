#include "hooks.h"
#include "mpsc/events_queue.h"
#include "amxx/pending_amx_object_store_impl.h"

void OnStartFramePre()
{
	ConsumeQueueEvents();
	DeleteUnfinishedPendingAmxObjectStore();
}