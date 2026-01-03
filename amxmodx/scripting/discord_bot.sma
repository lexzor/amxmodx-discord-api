#include <amxmodx>
#include <discordapi>

#define TOKEN "YOUR_DISCORD_BOT_TOKEN_HERE"
#define IDENTIFIER "discord_bot_example"

#pragma semicolon 1

public plugin_init()
{
    register_plugin("[DiscordAPI] Discord BOT", "0.1", "lexzor");

    if(!BotExists(IDENTIFIER))
    {
        if(!CreateBot(IDENTIFIER, TOKEN))
        {
            set_fail_state("Failed to create Discord BOT!");
            return;
        }
        
        if(!StartBot(IDENTIFIER))
        {
            set_fail_state("Failed to start Discord BOT!");
        }

        new opt[Options];
        opt[LOG_LEVEL] = DEFAULT;
        opt[PRINTS_EVENT_DATA] = false;
        formatex(opt[PREFIX], MAX_CONSOLE_PREFIX_LENGTH - 1, "[DiscordAPIChannelRelay]");
        SetBotOptions(IDENTIFIER, opt);
    }
    else 
    {
        log_amx("Discord BOT already exists.");
    }
}