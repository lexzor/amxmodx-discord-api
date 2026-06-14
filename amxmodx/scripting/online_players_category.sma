#include <amxmodx>
#include <discordapi>
#include <discordapibotguilds>

#pragma semicolon 1

#define IDENTIFIER "discord_bot"
#define GUILD_ID "1415670271611768966"
#define STATUS_CATEGORY_ID "1515621027688874166"

new Trie:g_tPlayerChannelData;
new Trie:g_tPendingChannelDeletes;

enum _:PlayerChannelData
{
    CHANNEL_ID[32],
    ChannelCreateHandle:CHANNEL_HANDLE,
    bool:RECEIVED_RESPONSE,
}

public plugin_init()
{
    register_plugin("[DiscordAPI] Channels", "0.1", "lexzor");

    if(IsBotReady(IDENTIFIER))
    {
        if(!GuildChannelExists(IDENTIFIER, GUILD_ID, STATUS_CATEGORY_ID))
        {
            log_amx("Channel does not exists");
        }
        else
        {
            new channelName[64];
            new channelParentID[32];
            if(!GetGuildChannel(IDENTIFIER, GUILD_ID, STATUS_CATEGORY_ID, channelName, charsmax(channelName), channelParentID, charsmax(channelParentID)))
            {
                set_fail_state("Category %s does not exists", STATUS_CATEGORY_ID);
            }
            else
            {
                log_amx("Channel found: name %s, parent_id: %s", channelName, channelParentID);
            }
        }
    }
    else log_amx("Bot %s not ready", IDENTIFIER);
}

public plugin_cfg()
{
    g_tPlayerChannelData = TrieCreate();
    g_tPendingChannelDeletes = TrieCreate();
}

public plugin_end()
{
    TrieDestroy(g_tPlayerChannelData);
    TrieDestroy(g_tPendingChannelDeletes);
}

public client_disconnected(id)
{
    if(!IsBotReady(IDENTIFIER))
        return;

    new authid[MAX_AUTHID_LENGTH];
    get_user_authid(id, authid, charsmax(authid));

    if(!TrieKeyExists(g_tPlayerChannelData, authid))
        return;

    new eData[PlayerChannelData];
    TrieGetArray(g_tPlayerChannelData, authid, eData, sizeof(eData));
    TrieDeleteKey(g_tPlayerChannelData, authid);

    if(eData[RECEIVED_RESPONSE] && eData[CHANNEL_ID][0])
    {
        if(!DeleteGuildChannel(IDENTIFIER, GUILD_ID, eData[CHANNEL_ID]))
        {
            log_amx("Failed to delete channel %s for %n", eData[CHANNEL_ID], id);
        }
    }
    else if(eData[CHANNEL_HANDLE] != INVALID_CHANNEL_HANDLE)
    {
        // Creation request is still in flight, mark it for deletion once OnGuildChannelCreate fires
        new key[12];
        num_to_str(_:eData[CHANNEL_HANDLE], key, charsmax(key));
        TrieSetCell(g_tPendingChannelDeletes, key, true);
    }

    return;
}

public client_putinserver(id)
{
    if(!IsBotReady(IDENTIFIER))
    {
        log_amx("Bot %s is not ready", IDENTIFIER);
        return;
    }

    if(!is_user_connected(id) || is_user_bot(id))
        return;

    new authid[MAX_AUTHID_LENGTH];
    get_user_authid(id, authid, charsmax(authid));

    // Clean up any stale entry left over from a previous session before creating a new channel
    if(TrieKeyExists(g_tPlayerChannelData, authid))
    {
        new eOld[PlayerChannelData];
        TrieGetArray(g_tPlayerChannelData, authid, eOld, sizeof(eOld));
        TrieDeleteKey(g_tPlayerChannelData, authid);

        if(eOld[RECEIVED_RESPONSE] && eOld[CHANNEL_ID][0])
        {
            if(!DeleteGuildChannel(IDENTIFIER, GUILD_ID, eOld[CHANNEL_ID]))
            {
                log_amx("Failed to delete stale channel %s for %n", eOld[CHANNEL_ID], id);
            }
        }
        else if(eOld[CHANNEL_HANDLE] != INVALID_CHANNEL_HANDLE)
        {
            new key[12];
            num_to_str(_:eOld[CHANNEL_HANDLE], key, charsmax(key));
            TrieSetCell(g_tPendingChannelDeletes, key, true);
        }
    }

    new const ChannelCreateHandle:channelHandle = BeginCreateGuildChannel(IDENTIFIER, GUILD_ID);

    if(channelHandle == INVALID_CHANNEL_HANDLE)
    {
        log_amx("Player %n won't be shown in Discord guild channel due to failure of creating a channel handle");
        return;
    }

    SetGuildChannelMemberString(channelHandle, NAME, fmt("%n", id));
    SetGuildChannelMemberString(channelHandle, PARENT_ID, STATUS_CATEGORY_ID);

    if(!EndCreateGuildChannel(IDENTIFIER, channelHandle))
    {
        log_amx("Failed to create guild channel for player %n", id);
        return;
    }

    new eData[PlayerChannelData];
    eData[CHANNEL_HANDLE] = channelHandle;
    eData[RECEIVED_RESPONSE] = false;

    TrieSetArray(g_tPlayerChannelData, authid, eData, sizeof(eData));

    return;
}

public OnGuildChannelCreate(const identifier[], const ChannelCreateHandle:channel_handle, const bool:success, const channel_id[])
{
    if(!equal(identifier, IDENTIFIER))
        return;

    if(!success)
    {
        log_amx("Failed to create channel");
        return;
    }

    if(channel_handle == INVALID_CHANNEL_HANDLE)
    {
        log_amx("OnGuildChannelCreate event received, but no channel_handle");
        return;
    }

    // If the player already disconnected before this completed, delete the channel and stop
    new pendingKey[12];
    num_to_str(_:channel_handle, pendingKey, charsmax(pendingKey));

    if(TrieKeyExists(g_tPendingChannelDeletes, pendingKey))
    {
        TrieDeleteKey(g_tPendingChannelDeletes, pendingKey);

        if(!DeleteGuildChannel(IDENTIFIER, GUILD_ID, channel_id))
        {
            log_amx("Failed to delete channel %s for disconnected player", channel_id);
        }

        return;
    }

    new data[PlayerChannelData];
    new TrieIter:iter = TrieIterCreate(g_tPlayerChannelData);

    while(!TrieIterEnded(iter))
    {
        TrieIterGetArray(iter, data, sizeof(data));

        if(data[CHANNEL_HANDLE] == channel_handle)
        {
            data[RECEIVED_RESPONSE] = true;
            copy(data[CHANNEL_ID], charsmax(data[CHANNEL_ID]), channel_id);
            data[CHANNEL_HANDLE] = INVALID_CHANNEL_HANDLE;

            new key[32];
            TrieIterGetKey(iter, key, charsmax(key));

            TrieSetArray(g_tPlayerChannelData, key, data, sizeof(data));

            break;
        }

        TrieIterNext(iter);
    }

    TrieIterDestroy(iter);

    return;
}

public OnGuildChannelDelete(const identifier[], const bool:success, const channel_id[])
{
    if(!equal(identifier, IDENTIFIER))
        return;

    if(!success)
    {
        log_amx("Failed to delete channel %s", channel_id);
        return;
    }

    server_print("Channel %s deleted successfully", channel_id);

    return;
}
