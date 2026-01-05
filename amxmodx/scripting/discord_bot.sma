#include <amxmodx>
#include <discordapi>
#include <json>

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
        opt[PRINT_EVENT_DATA] = false;
        formatex(opt[PREFIX], MAX_CONSOLE_PREFIX_LENGTH - 1, "[DiscordAPIChannelRelay]");
        SetBotOptions(IDENTIFIER, opt);
    }
    else 
    {
        log_amx("Discord BOT already exists.");
    }

    register_concmd("bot_guilds", "bot_guilds");
}

public bot_guilds(id)
{
    new buffer[700];
    new count = GetGuilds(IDENTIFIER, buffer, charsmax(buffer));

    if(!count)
    {
        if(id == 0)
        {
            server_print("There are not any guilds to be displayed!");
        }
        else 
        {
            client_print(0, print_console, "There are not any guilds to be displayed!");
        }

        return PLUGIN_HANDLED;
    }

    if(id == 0)
    {
        server_print("Guilds: %s", buffer);
    }
    else 
    {
        client_print(0, print_console, "Guilds: %s", buffer);
    }

    return PLUGIN_HANDLED;
}

public OnGuildCreated(const identifier[], const guild_data[])
{
    if(strcmp(identifier, IDENTIFIER) != 0)
    {
        return;
    }

    new JSON:guild = json_parse(guild_data);

    if(guild == Invalid_JSON)
    {
        log_amx("Failed to parse OnGuildCreated guild data: %s", guild_data);
        return;
    }

    enum OnGuildCreatedData
    {
        Id[64],
        Name[64]
    }

    new eData[OnGuildCreatedData];

    json_object_get_string(guild, "id", eData[Id], charsmax(eData[Id]));
    json_object_get_string(guild, "name", eData[Name], charsmax(eData[Name]));

    server_print("Bot has been added in guild %s (%s)", eData[Name], eData[Id]);
}

public OnGuildDeleted(const identifier[], const guild_data[])
{
    if(strcmp(identifier, IDENTIFIER) != 0)
    {
        return;
    }

    new JSON:guild = json_parse(guild_data);

    if(guild == Invalid_JSON)
    {
        log_amx("Failed to parse OnGuildCreated guild data: %s", guild_data);
        return;
    }

    enum OnGuildDeletedData
    {
        Id[64],
        Name[64],
        bool:Unavailable
    }

    new eData[OnGuildDeletedData];

    json_object_get_string(guild, "id", eData[Id], charsmax(eData[Id]));
    json_object_get_string(guild, "name", eData[Name], charsmax(eData[Name]));
    eData[Unavailable] = json_object_get_bool(guild, "unavailable");

    if(!eData[Unavailable])
    {
        server_print("Bot has been added in guild %s (%s)", eData[Name], eData[Id]);
    }
    else 
    {
        server_print("Server %s (%s) has become temporarly unavailable", eData[Name], eData[Id]);
    }
}