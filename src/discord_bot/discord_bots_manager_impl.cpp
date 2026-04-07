#include "precompiled.h"

#include "discord_bots_manager_impl.h"

std::unique_ptr<DiscordBotsManager> g_DiscordBotsManager = {};

void InitializeDiscordBotsManager()
{
    if(!g_DiscordBotsManager)
    {
        g_DiscordBotsManager = std::make_unique<DiscordBotsManager>();
    }
    else
    {
        g_DiscordBotsManager->ForEach([](DiscordBot& bot) {
            if(!bot.IsStarted())
            {
                try {
                    if(!bot.Start())
                        gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Failed to start bot");
                    else
                        gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Started bot %s", bot.GetIdentifier().c_str());
                } catch(const dpp::logic_exception& e) {
                    gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Failed to start bot. Error: %s\n", e.what());
                }
            }
        });
    }
}

void DeinitializeDiscordBotsManager()
{
    g_DiscordBotsManager->ForEach([](DiscordBot& bot) {
        if(bot.IsStarted())
        {
            try {
                if(!bot.Stop())
                    gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Failed to start bot");
                else
                    gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Stopped bot %s", bot.GetIdentifier().c_str());
            } catch (const dpp::logic_exception& e) {
                gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Failed to stop bot. Error: %s\n", e.what());
            }
        }
    });
}