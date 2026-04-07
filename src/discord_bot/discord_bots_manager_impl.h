#pragma once

#include <memory>

#include "discord_bot/discord_bot.h"
#include "discord_bot/discord_bots_manager.h"

extern std::unique_ptr<DiscordBotsManager> g_DiscordBotsManager;

void InitializeDiscordBotsManager();
void DeinitializeDiscordBotsManager();