#include <amxmodx>
#include <discordapi>

#define IDENTIFIER "discord_bot"

public plugin_init()
{
    register_plugin("[DiscordAPI] Show guild channels", "0.1", "lexzor");

    register_clcmd("show_guild_channels", "cmd_show_guild_channels", -1, "usage: show_guild_channels ^"guild_id^" from client console");
}

public cmd_show_guild_channels(id)
{
    if(!IsBotReady(IDENTIFIER))
    {
        server_print("Bot %s not ready", IDENTIFIER);
        return PLUGIN_HANDLED;
    }

    new guildId[32];
    read_argv(1, guildId, charsmax(guildId));

    server_print("Printing channels for guild id %s", guildId);

    new const channelsCount = GetGuildChannelsCount(IDENTIFIER, guildId);
    for(new i = 0, stringValue[64], integerValue; i < channelsCount; i++)
    {
        GetGuildChannelStringMemberByIndex(IDENTIFIER, guildId, i, DAPI_NAME, stringValue, charsmax(stringValue));
        server_print("String value %i: %s", i, stringValue);
        GetGuildChannelStringMemberByIndex(IDENTIFIER, guildId, i, DAPI_PARENT_ID, stringValue, charsmax(stringValue));
        server_print("String value %i: %s", i, stringValue);

        integerValue = GetGuildChannelIntegerMemberByIndex(IDENTIFIER, guildId, i, DAPI_TYPE);
        server_print("Integer value %i: %i", i, integerValue);
        integerValue = GetGuildChannelIntegerMemberByIndex(IDENTIFIER, guildId, i, DAPI_FLAGS);
        server_print("Integer value %i: %i", i, integerValue);
        integerValue = GetGuildChannelIntegerMemberByIndex(IDENTIFIER, guildId, i, DAPI_USER_LIMIT);
        server_print("Integer value %i: %i", i, integerValue);
    }

    return PLUGIN_HANDLED;
}