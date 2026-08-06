#pragma once

#include "precompiled.h"

cell AMX_NATIVE_CALL GetGuilds(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GuildChannelExistsById(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GuildChannelExistsByName(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL BeginCreateGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL EndCreateGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL DeleteGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SetGuildChannelMemberString(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SetGuildChannelMemberInteger(AMX* amx, cell* params);
cell AMX_NATIVE_CALL BeginEditGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL EndEditGuildChannel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GuildSlashCommandExists(AMX* amx, cell* params);
cell AMX_NATIVE_CALL BeginCreateGuildSlashCommand(AMX* amx, cell* params);
cell AMX_NATIVE_CALL EndCreateGuildSlashCommand(AMX* amx, cell* params);
cell AMX_NATIVE_CALL AddSlashCommandOption(AMX* amx, cell* params);
cell AMX_NATIVE_CALL BeginCreateSlashCommandOption(AMX* amx, cell* params);
cell AMX_NATIVE_CALL EndCreateSlashCommandOption(AMX* amx, cell* params);
cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceInteger(AMX* amx, cell* params);
cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceString(AMX* amx, cell* params);
cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceFloat(AMX* amx, cell* params);
cell AMX_NATIVE_CALL CreateGuildSlashCommand(AMX* amx, cell* params);
cell AMX_NATIVE_CALL DeleteGuildSlashCommand(AMX* amx, cell* params);