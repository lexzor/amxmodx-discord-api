#pragma once

#include <amxxmodule.h>

cell AMX_NATIVE_CALL CreateBot(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SetBotOptions(AMX* amxx, cell* params);
cell AMX_NATIVE_CALL BotExists(AMX* amx, cell* params);
cell AMX_NATIVE_CALL StartBot(AMX* amx, cell* params);
cell AMX_NATIVE_CALL StopBot(AMX* amx, cell* params);
cell AMX_NATIVE_CALL DeleteBot(AMX* amx, cell* params);
cell AMX_NATIVE_CALL RegisterGlobalSlashCommand(AMX* amx, cell* params);
cell AMX_NATIVE_CALL UnregisterGlobalSlashCommand(AMX* amx, cell* params);
cell AMX_NATIVE_CALL ClearGlobalSlashCommands(AMX* amx, cell* params);
cell AMX_NATIVE_CALL IsBotReady(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SendReply(AMX* amx, cell* params);