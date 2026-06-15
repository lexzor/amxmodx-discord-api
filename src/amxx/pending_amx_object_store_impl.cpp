#include "precompiled.h"
#include "pending_amx_object_store_impl.h"

std::unique_ptr<PendingAmxObjectStore> g_PendingAmxObjectStore = std::make_unique<PendingAmxObjectStore>();

void DeleteUnfinishedPendingAmxObjectStore()
{
	uint32_t deletedObjectsCount = g_PendingAmxObjectStore->Clear();

	if (deletedObjectsCount > 0)
	{
		MF_Log("[DiscordAPI] Warning: Deleted %i pending objects", deletedObjectsCount);
	}
}