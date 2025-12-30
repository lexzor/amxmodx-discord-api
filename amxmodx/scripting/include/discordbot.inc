#if defined _discordbot_included
	#endinput
#endif
#define _discordbot_included

#pragma reqlib discordbot
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordbot
#endif

/**
* Create a discord bot.
*
* @note Set index to 0 to send text globally.
*
* @param identifier         Bot identifier.
* @param token              Token string from Discord Developer Portal.
*
* @noreturn
*/
native BotHandleKey:CreateBot(const identifier[], const token[]);