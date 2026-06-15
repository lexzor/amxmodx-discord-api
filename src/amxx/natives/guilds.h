#pragma once

#include "precompiled.h"

cell AMX_NATIVE_CALL GetGuilds(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GuildChannelExists(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL BeginCreateGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL EndCreateGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL DeleteGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SetGuildChannelMemberString(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SetGuildChannelMemberInt(AMX* amx, cell* params);
cell AMX_NATIVE_CALL BeginEditGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL EndEditGuildChannel(AMX* amx, cell* params);