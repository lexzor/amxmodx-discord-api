#pragma once

#include "precompiled.h"

cell AMX_NATIVE_CALL GuildMemberExistsById(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GuildMemberExistsByUsername(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildMemberIdByUsername(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildMemberStringMemberById(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildMemberIntegerMemberById(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildMembersCount(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildMemberStringMemberByIndex(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildMemberIntegerMemberByIndex(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GuildMemberHasRole(AMX* amx, cell* params);
cell AMX_NATIVE_CALL AddGuildMemberRole(AMX* amx, cell* params);
cell AMX_NATIVE_CALL RemoveGuildMemberRole(AMX* amx, cell* params);
cell AMX_NATIVE_CALL FetchGuildUserById(AMX* amx, cell* params);
cell AMX_NATIVE_CALL FetchGuildUserByUsername(AMX* amx, cell* params);