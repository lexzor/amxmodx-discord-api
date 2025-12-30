#include <amxmodx>
#include <amxmisc>
#include <json>
#include <discordapi>

new const TOKEN[MAX_TOKEN_LENGTH]               = "YOUR_DISCORD_BOT_TOKEN_HERE"
new const DISCORD_ID[MAX_SNOWFLAKE_ID_LENGTH]   = "YOUR_DISCORD_BOT_USER_ID_HERE"
new const CHANNEL_ID[MAX_SNOWFLAKE_ID_LENGTH]   = "YOUR_CHANNEL_ID_HERE"

new const IDENTIFIER[MAX_IDENTIFIER_LENGTH]     = "DiscordAPIChannelRelay"

public plugin_init()
{
    register_plugin("[DiscordAPI] Chat Relay", "0.1", "lexzor")
    register_clcmd("say", "cmd_say")
    register_clcmd("say_team", "cmd_say")

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

        new opt[Options]
        opt[LOG_LEVEL] = DEFAULT
        formatex(opt[PREFIX], MAX_CONSOLE_PREFIX_LENGTH - 1, "[DiscordAPIChannelRelay]")
        SetBotOptions(IDENTIFIER, opt)
    }
    else 
    {
        log_amx("Discord BOT already exists.");
    }
}

public cmd_say(id)
{
    new message[128];	
    read_args(message, charsmax(message))
    remove_quotes(message)
    trim(message);

    if(strlen(message) > 0)
    {
        new name[MAX_NAME_LENGTH]
        get_user_name(id, name, sizeof(name));

        new discordMessage[128];
        formatex(discordMessage, charsmax(discordMessage), "[%s] %s: %s", get_user_team(id) == 1 ? "T" : "CT", name, message);

        SendMessageToChannel(IDENTIFIER, CHANNEL_ID, discordMessage);
    }
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
    json_object_get_string(d, "channel_id", channel_id, sizeof(channel_id));

    if(strcmp(channel_id, CHANNEL_ID) != 0)
    {
        goto cleanup;
    }

    new JSON:author = json_object_get_value(d, "author");

    new content[128];
    new authorName[MAX_NAME_LENGTH * 2];
    new authorId[64];
    
    json_object_get_string(author, "username", authorName, sizeof(authorName));
    json_object_get_string(d, "content", content, sizeof(content));
    json_object_get_string(author, "id", authorId, sizeof(authorId));

    if(strcmp(authorId, DISCORD_ID) == 0)
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