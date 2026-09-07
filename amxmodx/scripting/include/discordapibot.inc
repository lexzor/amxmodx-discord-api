/*
* Discord API Module for AMX Mod X. Below you will find all the natives, forwards and constants provided by the module. 
*
* Author: lexzor
* Website: https://github.com/lexzor/amxmodx-discord-api
*
*/

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#if defined _discordapibot_included
	#endinput
#endif

#define _discordapibot_included

/*
* Forwards
*
*/

/**
* Triggered after a bot has been created in Discord API.
*
* @note This function will be called after plugin_init for every
* 		bot which are already ready.
*
* @param identifier         Bot identifier.
*/
forward OnBotReady(const identifier[]);

/**
* Triggered when a bot it's stopped.
*
* @param identifier         Bot identifier.
*/
forward OnBotStopped(const identifier[]);

/**
* Triggered when a bot receives a log message from Discord API.
*
* @note This forward is always called for WARNING, ERROR and CRITICAL severities,
* 		regardless of the bot's configured LOG_LEVEL option. Below that (INFO, DEBUG,
* 		TRACE), it's called following the same rules used for console printing:
* 		not called at all if LOG_LEVEL is NONE, called only for INFO severity if
* 		LOG_LEVEL is DEFAULT, and called for every severity if LOG_LEVEL is VERBOSE.
*
* @param identifier         Bot identifier.
* @param severity           Log severity, a LogSeverity constant.
* @param message            Log message.
*/
forward OnBotLog(const identifier[], LogSeverity:severity, const message[]);

/*
*
* Natives
*
*/

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
* @return False if bot does not exists, true otherwise.
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
* Send reply on last slash command interaction.
*
* @note This function it's used to reply to an interaction.
*
* @param identifier         Bot identifier.
* @param reply              Text message to send.
*
* @return False if bot does not exists, is not ready or there is not any active interaction to reply at, true otherwise.
*/
native bool:SendReply(const identifier[], const reply[]);