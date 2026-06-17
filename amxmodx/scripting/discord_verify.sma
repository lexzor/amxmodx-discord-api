#include <amxmodx>
#include <discordapi>
#include <json>
#include <nvault>

#define IDENTIFIER "discord_bot"
#define GUILD_ID "1415670271611768966"

#pragma semicolon 1

enum _:DiscordAccountData
{
    USERNAME[64],
    ID[32],
    bool:LINKED
}

new const g_NVaultName[] = "discord_verify";
new g_NVaultHandle = INVALID_HANDLE;

new g_ePlayerDiscordAccountData[MAX_PLAYERS + 1][DiscordAccountData];

public plugin_init()
{
    register_plugin("[DiscordAPI] Account Verify", "0.1", "lexzor");

    if((g_NVaultHandle = nvault_open(g_NVaultName)) == INVALID_HANDLE)
    {
        set_fail_state("Failed to open %s NVault", g_NVaultName);
    }
}

public plugin_end()
{
    nvault_close(g_NVaultHandle);
}

public OnBotReady(const identifier[])
{
    if(!equal(identifier, IDENTIFIER))
        return;

    new slashCommandId[32];
    if(!GuildSlashCommandExists(IDENTIFIER, GUILD_ID, "verify", slashCommandId, charsmax(slashCommandId)))
    {
        new const SlashCommandHandle:slashCommandHandle = BeginCreateGuildSlashCommand(IDENTIFIER, "verify", "Link Discord account to SteamID.");
        AddSlashCommandOption(
            slashCommandHandle,
            STRING,
            "steamid",
            "Your SteamID. You can take it by typing 'status' in console. Must be online on server!",
            true
        );

        new const SlashCommandOptionHandle:slashCommandOptionHandle = BeginCreateSlashCommandOption(STRING, "origin", "Where have you heard about us?", true);
        AddSlashCommandOptionChoiceString(slashCommandOptionHandle, "Youtube", "Youtube");
        AddSlashCommandOptionChoiceString(slashCommandOptionHandle, "Tiktok", "Tiktok");
        AddSlashCommandOptionChoiceString(slashCommandOptionHandle, "Kick", "Kick");
        AddSlashCommandOptionChoiceString(slashCommandOptionHandle, "GameTracker", "GameTracker");
        EndCreateSlashCommandOption(slashCommandOptionHandle, slashCommandHandle);

        EndCreateGuildSlashCommand(IDENTIFIER, slashCommandHandle, GUILD_ID);
    }
    else
    {
        server_print("Slash command already registered. ID: %s", slashCommandId);
    }

    register_concmd("delete_verify_slash_command", "DeleteVerifySlashCommand", FCVAR_SERVER, "Delete /verify slash command to create a new one");

    return;
}

public DeleteVerifySlashCommand(id)
{
    new slash_command_id[32];
    read_argv(1, slash_command_id, charsmax(slash_command_id));

    if(!DeleteGuildSlashCommand(IDENTIFIER, GUILD_ID, slash_command_id))
    {
        log_amx("Failed to send delete request for /verify guild slash command");
    }
    else server_print("Delete request for /verify has been sent successfully");
}

public OnGuildSlashCommandCreate(const identifier[], const bool:success, const name[], const slash_command_id[])
{
    log_amx("%s: Command %s (%s) %s", identifier, name, slash_command_id, success ? "created" : "failed to create");
}

public OnGuildSlashCommandDelete(const identifier[], const bool:success, const name[], const slash_command_id[])
{
    log_amx("%s: Command %s (%s) %s", identifier, name, slash_command_id, success ? "deleted" : "failed to delete");
}

public OnGuildSlashCommand(const identifier[], const guild_id[], const name[], const caller_id[], const caller_name[], const options[], const channel_id[])
{
    if(!equal(IDENTIFIER, identifier) || !equal(GUILD_ID, guild_id) || !equal(name, "verify"))
        return;

    server_print("%s called /verify guild slash command", caller_name);

    new JSON:optionsJSON = json_parse(options, false, false);

    if(optionsJSON == Invalid_JSON)
    {
        log_amx("OnGuildSlashCommand: Failed to parse '%s' JSON (len: %i)", options, strlen(options));
        SendReply(IDENTIFIER, "An error has been occured on the game server side. Please contact administration!");
        return;
    }

    enum _:CommandOptions
    {
        steamid[MAX_AUTHID_LENGTH],
        origin[32]
    };

    new options[CommandOptions];
    json_object_get_string(optionsJSON, "steamid", options[steamid], charsmax(options[steamid]));
    json_object_get_string(optionsJSON, "origin", options[origin], charsmax(options[origin]));

    new players[MAX_PLAYERS], num;
    get_players(players, num, "ch");

    new foundPlayerId = -1;

    for(new i = 0, authid[MAX_AUTHID_LENGTH]; i < num; i++)
    {
        new player = players[i];

        if(!is_user_connected(player))
            continue;

        get_user_authid(player, authid, charsmax(authid));

        if(equal(authid, options[steamid]))
        {
            foundPlayerId = player;
            break;
        }
    }

    if(foundPlayerId == -1)
    {
        SendReply(IDENTIFIER, "No player found with specified SteamID. Please join server, type `status` in console, copy your `SteamID` then use the command when online.");
        return;
    }

    if(g_ePlayerDiscordAccountData[foundPlayerId][LINKED])
    {
        SendReply(IDENTIFIER, fmt("SteamID `%s` already verified!", options[steamid]));
        client_print_color(foundPlayerId, print_team_default, "^4[DiscordAPI]^1 SteamID already verified. Discord member^3 %s^1 tried to reverify", caller_name);
        return;
    }

    nvault_set(g_NVaultHandle, options[steamid], fmt("^"%s^" ^"%s^" ^"%s^"", caller_id, caller_name, options[origin]));

    client_print_color(0, print_team_default, "^4[DiscordAPI]^3 %n^1 linked^4 Discord^1 account (%s)", foundPlayerId, caller_name);

    SendReply(IDENTIFIER, fmt("SteamID `%s` verified successfully!", options[steamid]));

    copy(g_ePlayerDiscordAccountData[foundPlayerId][ID], charsmax(g_ePlayerDiscordAccountData[][ID]), caller_id);
    copy(g_ePlayerDiscordAccountData[foundPlayerId][USERNAME], charsmax(g_ePlayerDiscordAccountData[][USERNAME]), caller_id);
    g_ePlayerDiscordAccountData[foundPlayerId][LINKED] = true;

    return;
}

public client_putinserver(id)
{
    new authid[MAX_AUTHID_LENGTH];
    get_user_authid(id, authid, charsmax(authid));

    arrayset(g_ePlayerDiscordAccountData[id][ID], 0x00, charsmax(g_ePlayerDiscordAccountData[][ID]));
    arrayset(g_ePlayerDiscordAccountData[id][USERNAME], 0x00, charsmax(g_ePlayerDiscordAccountData[][USERNAME]));
    g_ePlayerDiscordAccountData[id][LINKED] = false;

    new data[128], ts;
    if(nvault_lookup(g_NVaultHandle, authid, data, charsmax(data), ts))
    {
        new temp[2][64];
        parse(data, temp[0], charsmax(temp[]), temp[1], charsmax(temp[]));

        copy(g_ePlayerDiscordAccountData[id][ID], charsmax(g_ePlayerDiscordAccountData[][ID]), temp[0]);
        copy(g_ePlayerDiscordAccountData[id][USERNAME], charsmax(g_ePlayerDiscordAccountData[][USERNAME]), temp[1]);
    
        g_ePlayerDiscordAccountData[id][LINKED] = true;
    }
}