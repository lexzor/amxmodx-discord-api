#include <amxmodx>
#include <discordapi>

#pragma semicolon 1

#define IDENTIFIER "discord_bot"
#define GUILD_ID "1415670271611768966"

public plugin_init()
{
    register_plugin("[DiscordAPI] Player list guild slash command", "0.1", "lexzor");
}

public OnBotReady(const identifier[])
{
    if(!GuildSlashCommandExists(IDENTIFIER, GUILD_ID, "playerslist"))
    {
        CreateGuildSlashCommand(IDENTIFIER, GUILD_ID, "playerslist", "List of online players");
    }
}

public OnGuildSlashCommand(const identifier[], const guild_id[], const name[], const caller_id[], const caller_name[], const options[], const channel_id[])
{
    if(!equal(identifier, IDENTIFIER) || !equal(guild_id, GUILD_ID) || !equal(name, "playerslist"))
        return;

    if(cvar_exists("discord_bot_chat_relay_channel"))
    {
        new chatRelayChannelId[32];
        get_cvar_string("discord_bot_chat_relay_channel", chatRelayChannelId, charsmax(chatRelayChannelId));

        if(!equal(chatRelayChannelId, channel_id))
        {
            SendReply(IDENTIFIER, fmt("This command can be used only in <#%s> channel!", chatRelayChannelId));
            return;
        }
    }

    PrintPlayersList();
    
    client_print_color(0, print_team_default, "^4[DiscordAPI]^3 %s^1 called ^4/playerslist^1, maybe you say hello to him!", caller_name);
}

PrintPlayersList()
{
    static message[2048];
    new len = 0;

    message[0] = 0;

    len += formatex(message[len], charsmax(message) - len, "```text^n");
    len += formatex(message[len], charsmax(message) - len,
        "%-2s %-20s %-6s %-6s %-4s^n",
        "#", "Name", "Frags", "Deaths", "Ping");

    new players[MAX_PLAYERS], num;
    get_players(players, num, "ch");

    for(new i = 0; i < num; i++)
    {
        new player = players[i];

        new name[32];
        get_user_name(player, name, charsmax(name));

        new ping, loss;
        get_user_ping(player, ping, loss);

        len += formatex(message[len], charsmax(message) - len,
            "%-2d %-20.20s %-6d %-6d %-4d^n",
            i + 1,
            name,
            get_user_frags(player),
            get_user_deaths(player),
            ping);
    }

    len += formatex(message[len], charsmax(message) - len, "```");

    SendReply(IDENTIFIER, message);
}