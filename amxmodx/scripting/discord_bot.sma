#include <amxmodx>
#include <amxmisc>
#include <discordapi>
#include <json>

// Debug logs for library API
#define DEBUG

#define IDENTIFIER "discord_bot"

#define PLUGIN_CONFIG "discord_bot.cfg"

#pragma semicolon 1

enum CVARS
{
    TOKEN[128]
}

new g_eCvar[CVARS];

public plugin_init()
{
    register_plugin("[DiscordAPI] Discord BOT", "0.1", "lexzor");

    bind_pcvar_string(
		create_cvar(
            "discord_bot_token",
            "bot_token",
            FCVAR_PROTECTED | FCVAR_SPONLY | FCVAR_SERVER,
            "Discord bot token"
		),
		g_eCvar[TOKEN],
        charsmax(g_eCvar[TOKEN])
	);

    new szCfgDir[64];
    get_configsdir(szCfgDir, charsmax(szCfgDir));
    server_cmd("exec %s", fmt("%s/%s", szCfgDir, PLUGIN_CONFIG));
    server_exec();

    if(!BotExists(IDENTIFIER))
    {
        if(!CreateBot(IDENTIFIER, g_eCvar[TOKEN]))
        {
            set_fail_state("Failed to create Discord BOT!");
            return;
        }
        
        new opt[Options];
        opt[LOG_LEVEL] = DEFAULT;
        opt[PRINT_EVENT_DATA] = false;
        formatex(opt[PREFIX], MAX_CONSOLE_PREFIX_LENGTH - 1, "[DiscordBOT]");
        SetBotOptions(IDENTIFIER, opt);

        log_amx("Bot %s created", IDENTIFIER);
    }
#if defined DEBUG   
    else log_amx("Bot %s does not exists", IDENTIFIER);
#endif

    if(!IsBotReady(IDENTIFIER))
    {
        if(!StartBot(IDENTIFIER))
        {
            set_fail_state("Failed to start Discord BOT!");
        }

        log_amx("Trying to start bot %s", IDENTIFIER);
    }
#if defined DEBUG   
    else log_amx("Bot %s already started", IDENTIFIER);
#endif

    register_concmd("bot_guilds", "bot_guilds");
}

public OnBotReady(const identifier[])
{
    if(equal(identifier, IDENTIFIER))
    {
        log_amx("Bot %s started", IDENTIFIER);
    }
#if defined DEBUG
    else log_amx("OnBotReady received for %s, but it's not plugin's bot %s", identifier, IDENTIFIER);
#endif
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
            client_print(id, print_console, "There are not any guilds to be displayed!");
        }

        return PLUGIN_HANDLED;
    }

    if(id == 0)
    {
        server_print("Guilds: %s", buffer);
    }
    else 
    {
        client_print(id, print_console, "Guilds: %s", buffer);
    }

    return PLUGIN_HANDLED;
}

public OnGuildCreated(const identifier[], const guild_data[])
{
    if(!equal(identifier, IDENTIFIER))
    {
        return;
    }
#if defined DEBUG
    else log_amx("OnBotReady received for %s, but it's not plugin's bot %s", identifier, IDENTIFIER);
#endif

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
    if(equal(identifier, IDENTIFIER))
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