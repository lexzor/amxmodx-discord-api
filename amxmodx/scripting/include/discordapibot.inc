/*
* Discord API Module for AMX Mod X. Below you will find all the natives and forwards provided by the module. 
*
* Author: lexzor
* Website: https://github.com/lexzor/amxmodx-discord-api
*
*/

#if defined _discordapi_included
	#endinput
#endif

#define _discordapi_included

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#include <discordapibotchannels>
#include <discordapibotguilds>

/*
* Forwards
*
*/

/**
* Triggered after a bot has been created in Discord API.
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
* Register a slash command.
*
* @note Callback params: func(const identifier[]). 
* @note Callback function it's an interaction.
*
* @param identifier         Bot identifier.
* @param command            Command name.
* @param description        Command description.
* @param callback			Public function name to be called when command is used.
*
* @return False if bot does not exists or is not ready, true otherwise.
*/
native bool:RegisterGlobalSlashCommand(const identifier[], const command[], const description[], const callback[]);

/**
* Unregister a slash command.
*
* @param identifier         Bot identifier.
* @param command            Command name.
*
* @return False if bot does not exists or is not ready, true otherwise.
*/
native bool:UnregisterGlobalSlashCommand(const identifier[], const command[]);

/**
* Delete all slash commands which has been registered for a bot.
*
* @param identifier         Bot identifier.
*
* @return False if bot does not exists or is not ready, true otherwise.
*/
native bool:ClearGlobalSlashCommands(const identifier[]);

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