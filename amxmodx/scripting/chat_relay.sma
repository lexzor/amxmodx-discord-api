#include <amxmodx>
#include <amxmisc>
#include <json>
#include <discordapi>

#define IDENTIFIER "discord_bot_example"
#define BOT_ID "YOUR_DISCORD_BOT_ID"
#define CHANNEL_ID "YOUR_CHANNEL_ID_HERE"

#pragma semicolon 1

public plugin_init()
{
    register_plugin("[DiscordAPI] Chat Relay", "0.1", "lexzor");
    
    register_clcmd("say", "cmd_say");
    register_clcmd("say_team", "cmd_say");
}

public cmd_say(id)
{
    if(!IsBotReady(IDENTIFIER))
    {
        return PLUGIN_CONTINUE;
    }

    new message[128];	
    read_args(message, charsmax(message));
    remove_quotes(message);
    trim(message);

    if(strlen(message) == 0)
    {
        return PLUGIN_CONTINUE;
    }

    new name[MAX_NAME_LENGTH + 1];
    get_user_name(id, name, charsmax(name));

    new discordMessage[128];
    formatex(discordMessage, charsmax(discordMessage), "[%s] %s: %s", get_user_team(id) == 1 ? "T" : "CT", name, message);

    SendMessageToChannel(IDENTIFIER, CHANNEL_ID, discordMessage);

    return PLUGIN_CONTINUE;
}

public OnBotReady(const identifier[])
{
    if(!strcmp(identifier, IDENTIFIER, false))
    {
        log_amx("Bot %s is ready!", IDENTIFIER);
    }
}

public OnMessageCreated(const identifier[], const raw_json_event[])
{
    if(strcmp(identifier, IDENTIFIER, false) != 0)
    {
        return;
    }

    new JSON:jsonEvent = json_parse(raw_json_event);

    if(jsonEvent == Invalid_JSON)
    {
        log_amx("Failed to parse raw json event from OnMessageCreated.");
        return;
    }

    new JSON:d = json_object_get_value(jsonEvent, "d");
    new channel_id[64];
    json_object_get_string(d, "channel_id", channel_id, charsmax(channel_id));

    if(strcmp(channel_id, CHANNEL_ID) != 0)
    {
        goto cleanup;
    }

    new JSON:author = json_object_get_value(d, "author");

    new content[128];
    new authorName[MAX_NAME_LENGTH * 2];
    new authorId[64];
    
    json_object_get_string(author, "username", authorName, charsmax(authorName));
    json_object_get_string(d, "content", content, charsmax(content));
    json_object_get_string(author, "id", authorId, charsmax(authorId));

    if(strcmp(authorId, BOT_ID) == 0)
    {
        goto cleanup;
    }

    new message[128];
    formatex(message, charsmax(message), "^4[Discord]^1 %s^4:^1 %s", authorName, content);
    client_print_color(0, print_team_default, message);
    server_print(message);
    
cleanup:
    json_free(d);
    json_free(author);
    json_free(jsonEvent);
}