#include "precompiled.h"

#include "discord_bots_manager_impl.h"

std::unique_ptr<DiscordBotsManager> g_DiscordBotsManager = nullptr;

void InitializeDiscordBotsManager()
{
    if(!g_DiscordBotsManager)
    {
        g_DiscordBotsManager = std::make_unique<DiscordBotsManager>();
    }
}

void DeinitializeDiscordBotsManager()
{
    g_DiscordBotsManager->ForEach([](DiscordBot& bot) {
        if(bot.GetReadyState())
        {
            bot.Stop();
        }
    });
}