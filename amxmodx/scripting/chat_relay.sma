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

    if(equal(command, "say_team") && message[0] == '@')
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
    if(!equal(identifier, IDENTIFIER))
    {
        log_amx("Bot %s is ready!", IDENTIFIER);
    }
}

public OnChannelMessageCreated(const identifier[], const channel_id[], const event_data[])
{
    if(!equal(identifier, IDENTIFIER))
    {
        return;
    }

    new JSON:jsonEvent = json_parse(event_data);

    if(jsonEvent == Invalid_JSON)
    {
        log_amx("Failed to parse raw json event from OnChannelMessageCreated.");
        return;
    }

    if(!equal(channel_id, CHANNEL_ID))
    {
        goto cleanup;
    }

    new JSON:author = json_object_get_value(jsonEvent, "author");

    new content[128];
    new authorName[MAX_NAME_LENGTH * 2];
    new authorId[64];
    
    json_object_get_string(author, "username", authorName, charsmax(authorName));
    json_object_get_string(jsonEvent, "content", content, charsmax(content));
    json_object_get_string(author, "id", authorId, charsmax(authorId));

    if(equal(authorId, BOT_ID))
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