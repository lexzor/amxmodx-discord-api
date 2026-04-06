#include <amxmodx>
#include <amxmisc>
#include <json>
#include <discordapi>

#define IDENTIFIER "discord_bot"

#pragma semicolon 1

enum CVARS
{
    ID[64],
    CHANNEL[64],
    BLOCKED_MSGS_PREFIXES[64]
}

new g_eCvar[CVARS];

public plugin_init()
{
    register_plugin("[DiscordAPI] Chat Relay", "0.7", "lexzor");
    
    register_clcmd("say", "cmd_say");
    register_clcmd("say_team", "cmd_say");

    bind_pcvar_string(
		create_cvar(
            "discord_bot_id",
            "channel_id",
            FCVAR_PROTECTED | FCVAR_SPONLY | FCVAR_SERVER,
            "Discord bot ID. Used to ignore messages from itself"
		),
		g_eCvar[ID],
        charsmax(g_eCvar[ID])
	);

    bind_pcvar_string(
		create_cvar(
            "discord_bot_chat_relay_channel",
            "channel_id",
            FCVAR_PROTECTED | FCVAR_SPONLY | FCVAR_SERVER,
            "Channel to send and read messages"
		),
		g_eCvar[CHANNEL],
        charsmax(g_eCvar[CHANNEL])
	);

    bind_pcvar_string(
        create_cvar(
            "discord_bot_chat_relay_blocked_msgs_prefixes",
            "channel_id",
            FCVAR_PROTECTED | FCVAR_SPONLY | FCVAR_SERVER,
            "Chat messages prefixes to block (used for chat commands)"
		),
		g_eCvar[BLOCKED_MSGS_PREFIXES],
        charsmax(g_eCvar[BLOCKED_MSGS_PREFIXES])
    );
}

public cmd_say(id)
{
    if(!IsBotReady(IDENTIFIER))
    {
        return PLUGIN_CONTINUE;
    }
    
    new command[64];
    read_argv(0, command, charsmax(command));

    new message[128];	
    read_args(message, charsmax(message));
    remove_quotes(message);
    trim(message);

    if(strlen(message) == 0)
    {
        return PLUGIN_CONTINUE;
    }

    if(equal(command, "say_team"))
    {
        return PLUGIN_CONTINUE;
    }

    new blockedMessagesPrefixes[128];
    copy(blockedMessagesPrefixes, charsmax(blockedMessagesPrefixes), g_eCvar[BLOCKED_MSGS_PREFIXES]);

    new prefix[16];
    while(blockedMessagesPrefixes[0] && strtok2(blockedMessagesPrefixes, prefix, charsmax(prefix), blockedMessagesPrefixes, charsmax(blockedMessagesPrefixes), ',') != -1)
    {
        if(strncmp(message, prefix, strlen(prefix)) == 0)
        {
            return PLUGIN_CONTINUE;
        }
    }

    new name[MAX_NAME_LENGTH + 1];
    get_user_name(id, name, charsmax(name));

    new discordMessage[128];
    formatex(discordMessage, charsmax(discordMessage), "[%s] %s: %s", get_user_team(id) == 1 ? "T" : "CT", name, message);

    SendMessageToChannel(IDENTIFIER, g_eCvar[CHANNEL], discordMessage);

    return PLUGIN_CONTINUE;
}

public OnChannelMessageCreated(const identifier[], const channel_id[], const event_data[])
{
    if(!equal(identifier, IDENTIFIER) || !equal(channel_id, g_eCvar[CHANNEL]))
        return;

    new JSON:jsonEvent = json_parse(event_data);

    if(jsonEvent == Invalid_JSON)
    {
        log_amx("Failed to parse raw json event from OnChannelMessageCreated.");
        return;
    }

    new JSON:author = json_object_get_value(jsonEvent, "author");

    new content[128];
    new authorName[MAX_NAME_LENGTH * 2];
    new authorId[64];
    
    json_object_get_string(author, "username", authorName, charsmax(authorName));
    json_object_get_string(jsonEvent, "content", content, charsmax(content));
    json_object_get_string(author, "id", authorId, charsmax(authorId));

    if(equal(authorId, g_eCvar[ID]))
    {
        goto cleanup;
    }

    new JSON:mentionsArray = json_object_get_value(jsonEvent, "mentions");
    new count = json_array_get_count(mentionsArray);

    if(count > 0)
    {
        enum MentionData
        {
            Username[MAX_NAME_LENGTH],
            Id[32],
        }

        new JSON:mention;

        for(new i = 0, data[MentionData], mentionString[64]; i < count; i++)
        {
            mention = json_array_get_value(mentionsArray, i);
            json_object_get_string(mention, "username", data[Username], charsmax(data[Username]));
            json_object_get_string(mention, "id", data[Id], charsmax(data[Id]));

            formatex(mentionString, charsmax(mentionString), "<@%s>", data[Id]);
            new const pos = strfind(content, mentionString);

            if(pos == -1)
            {
                continue;
            }

            replace(content, charsmax(content), mentionString, fmt("^4@%s^1", data[Username]));
        }
    
        json_free(mention);
    }

    new message[128];
    formatex(message, charsmax(message), "^4[Discord]^1 %s^4:^1 %s", authorName, content);
    client_print_color(0, print_team_default, message);
    server_print(message);
    
cleanup:
    json_free(author);
    json_free(jsonEvent);
}