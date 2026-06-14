#pragma once

#include <memory>

#include "pending_amx_object_store.h"

extern std::unique_ptr<PendingAmxObjectStore> g_PendingAmxObjectStore;

void DeleteUnfinishedPendingAmxObjectStore();