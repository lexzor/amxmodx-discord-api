#if defined _discordapi_included
	#endinput
#endif

#define _discordapi_included

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

// #include <discordapiguild.inc>

const MAX_TOKEN_LENGTH 				= 128;
const MAX_IDENTIFIER_LENGTH			= 32;
const MAX_CONSOLE_PREFIX_LENGTH 	= 32;
const MAX_GUILD_ID_LENGTH			= 64;

enum LogLevel (+=1)
{
	NONE = 0,
	DEFAULT,
	VERBOSE
}

enum Options
{
	LogLevel:LOG_LEVEL,
	PREFIX[MAX_CONSOLE_PREFIX_LENGTH],
}

/**
* Create a discord bot.
*
* @param identifier         Bot identifier.
* @param token              Token string from Discord Developer Portal.
*
* @return False if any error it's encountered, true if the bot has been created or already exists.
*/
native bool:CreateBot(const identifier[], const token[]);

/**
* Set bot options.
*
* @param identifier         Bot identifier.
* @param options            Array of Options enum
*
* @noreturn
*/
native SetBotOptions(const identifier[], const array[Options]);

/**
* Start a created bot.
*
* @note This native can be called without mention any bot options.
*
* @param identifier         Bot identifier.
*
* @return True if started successfully, false otherwise.
*/
native bool:StartBot(const identifier[]);

/**
* Stop a bot.
*
* @note Called to stop the bot, not to delete it.
*
* @param identifier         Bot identifier.
*
* @return True if stopped successfully, false otherwise.
*/
native bool:StopBot(const identifier[]);

/**
* Delete a bot.
*
* @note Calling this native will delete bot from module.
*
* @param identifier         Bot identifier.
*
* @return True if deleted successfully, false otherwise.
*/
native bool:DeleteBot(const identifier[]);

/**
* Check if a bot already exist for the provided identifier.
*
* @param identifier         Bot identifier.
*
* @return True if exists, false otherwise.
*/
native bool:BotExists(const identifier[]);

/**
* Check if a bot it's connected to Discord API.
*
* @param identifier         Bot identifier.
*
* @return True if exists, false otherwise.
*/
native bool:IsBotReady(const identifier[]);

/**
* Register a slash command.
*
* @note Callback params: func(const identifier[]). 
*
* @param identifier         Bot identifier.
* @param command            Command name.
* @param description        Command description.
* @param callback			Public function name to be called when command is used.
*
* @return True if command has been registered successfully, false otherwise.
*/
native RegisterGlobalSlashCommand(const identifier[], const command[], const description[], const callback[]);

/**
* Unregister a slash command.
*
* @param identifier         Bot identifier.
* @param command         Channel ID.
*
* @return True if command has been unregistered successfully, false otherwise.
*/
native UnregisterGlobalSlashCommand(const identifier[], const command[]);

/**
* Delete all slash commands which has been registered for a bot.
*
* @param identifier         Bot identifier.
*
* @return True if every registered command has been unregistered successfully, false otherwise.
*/
native ClearGlobalSlashCommands(const identifier[]);

/**
* Send reply on last slash command interaction.
*
* @param identifier         Bot identifier.
*/
native SendReply(const identifier[], const reply[]);

/**
* Send a message to a channel.
*
* @param identifier         Bot identifier.
* @param channel_id         Channel ID.
* @param message			Text message to send.
*/
native SendMessageToChannel(const identifier[], const channel_id[], const message[]);

/**
* Triggered after a bot has been created in Discord API.
*
* @param identifier         Bot identifier.
*/
forward OnBotReady(const identifier[]);

/**
* Triggered when a message is created.
*
* @param identifier         Bot identifier.
* @param raw_json_event     Raw JSON event string (provided by Discord API).
*/
forward OnMessageCreated(const identifier[], const raw_json_event[]);