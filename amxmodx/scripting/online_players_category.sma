/**
 * Please note this plugin it's used to test the module's channels API.
 * Using this plugin on your server will hit Discord API rate limiting.
 */

#include <amxmodx>
#include <discordapi>
#include <discordapibotguilds>

#pragma semicolon 1

#define IDENTIFIER "discord_bot"
#define GUILD_ID "1415670271611768966"
#define STATUS_CATEGORY_ID "1515621027688874166"

#define TICK_RATE 1.0

enum _:QueueType
{
    Q_CREATE,
    Q_DELETE,
    Q_RENAME_CATEGORY,
    Q_RENAME_CHANNEL
}

enum _:QueueItem
{
    QueueType:QT_TYPE,
    QT_ID[32],
    QT_NAME[64],
    ChannelHandle:QT_HANDLE,
    QT_AUTH[32]
}

new Array:g_aQueue;
new Trie:g_tPlayerChannelData;

new bool:g_bCategoryDirty;
new g_iLastPlayers;

enum _:PlayerChannelData
{
    CHANNEL_ID[32],
    ChannelHandle:CHANNEL_HANDLE,
    bool:RECEIVED_RESPONSE
}

public plugin_init()
{
    register_plugin("[DiscordAPI] Online Players Category", "1.0", "lexzor + Claude Code");

    g_aQueue = ArrayCreate(QueueItem);
    g_tPlayerChannelData = TrieCreate();

    set_task(TICK_RATE, "ProcessQueue", _, _, _, "b");
}

public plugin_end()
{
    ProcessQueue();
    ArrayDestroy(g_aQueue);
    TrieDestroy(g_tPlayerChannelData);
}

public client_putinserver(id)
{
    if(!IsBotReady(IDENTIFIER) || is_user_bot(id))
        return;

    new authid[32];
    get_user_authid(id, authid, charsmax(authid));

    g_bCategoryDirty = true;

    new item[QueueItem];
    item[QT_TYPE] = Q_CREATE;
    copy(item[QT_AUTH], charsmax(item[QT_AUTH]), authid);
    copy(item[QT_NAME], charsmax(item[QT_NAME]), fmt("%n", id));

    ArrayPushArray(g_aQueue, item);
}

public client_disconnected(id)
{
    if(!IsBotReady(IDENTIFIER))
        return;

    new authid[32];
    get_user_authid(id, authid, charsmax(authid));

    g_bCategoryDirty = true;

    new data[PlayerChannelData];

    if(TrieGetArray(g_tPlayerChannelData, authid, data, sizeof(data)))
    {
        new item[QueueItem];
        item[QT_TYPE] = Q_DELETE;
        copy(item[QT_ID], charsmax(item[QT_ID]), data[CHANNEL_ID]);

        ArrayPushArray(g_aQueue, item);
        TrieDeleteKey(g_tPlayerChannelData, authid);
    }
}

public OnGuildChannelCreate(const identifier[], const ChannelHandle:handle, const bool:success, const channel_id[])
{
    if(!equal(identifier, IDENTIFIER) || !success)
        return;

    new authid[32];
    num_to_str(_:handle, authid, charsmax(authid));

    new data[PlayerChannelData];
    new TrieIter:iter = TrieIterCreate(g_tPlayerChannelData);

    while(!TrieIterEnded(iter))
    {
        TrieIterGetArray(iter, data, sizeof(data));

        if(data[CHANNEL_HANDLE] == handle)
        {
            copy(data[CHANNEL_ID], charsmax(data[CHANNEL_ID]), channel_id);
            data[RECEIVED_RESPONSE] = true;

            new key[32];
            TrieIterGetKey(iter, key, charsmax(key));

            TrieSetArray(g_tPlayerChannelData, key, data, sizeof(data));
            break;
        }

        TrieIterNext(iter);
    }

    TrieIterDestroy(iter);
}

public ProcessQueue()
{
    if(!IsBotReady(IDENTIFIER))
        return;

    new item[QueueItem];

    if(ArraySize(g_aQueue) == 0)
    {
        if(g_bCategoryDirty)
        {
            UpdateCategory();
            g_bCategoryDirty = false;
        }
        return;
    }

    ArrayGetArray(g_aQueue, 0, item);
    ArrayDeleteItem(g_aQueue, 0);

    switch(item[QT_TYPE])
    {
        case Q_CREATE:
        {
            new const ChannelHandle:handle =
                BeginCreateGuildChannel(IDENTIFIER, GUILD_ID);

            if(handle == INVALID_CHANNEL_HANDLE)
                return;

            SetGuildChannelMemberString(handle, NAME, item[QT_NAME]);
            SetGuildChannelMemberString(handle, PARENT_ID, STATUS_CATEGORY_ID);
            SetGuildChannelMemberInteger(handle, TYPE, CHANNEL_VOICE);
            SetGuildChannelMemberInteger(handle, USER_LIMIT, 0);

            if(!EndCreateGuildChannel(IDENTIFIER, handle))
                return;

            new data[PlayerChannelData];
            data[CHANNEL_HANDLE] = handle;

            TrieSetArray(g_tPlayerChannelData, item[QT_AUTH], data, sizeof(data));
        }

        case Q_DELETE:
        {
            DeleteGuildChannel(IDENTIFIER, GUILD_ID, item[QT_ID]);
        }
    }
}

UpdateCategory()
{
    new players[MAX_PLAYERS], num;
    get_players(players, num, "ch");

    if(num == g_iLastPlayers)
        return;

    g_iLastPlayers = num;

    new const ChannelHandle:cat =
        BeginEditGuildChannel(IDENTIFIER, STATUS_CATEGORY_ID);

    if(cat == INVALID_CHANNEL_HANDLE)
        return;

    SetGuildChannelMemberString(cat, NAME, fmt("Online Players - %d", num));
    EndEditGuildChannel(IDENTIFIER, cat);
}