/*
* Discord API Module for AMX Mod X. Below you will find constats used in AMXX plugins and Metamod plugin. 
*
* Author: lexzor
* Website: https://github.com/lexzor/amxmodx-discord-api
*
*/

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#if defined _discordapiconsts_included
	#endinput
#endif
#define _discordapiconsts_included

#define DISCORD_API_AMXX_API_VERSION_STRING "1.4.5"

const MAX_IDENTIFIER_LENGTH			= 32;
const MAX_CONSOLE_PREFIX_LENGTH 	= 32;


// Log level of the module
enum LogLevel
{
	NONE,
	DEFAULT,
	VERBOSE
}

// Enum used to create an array which will be passed to native SetBotOptions
enum Options
{
	LogLevel:LOG_LEVEL,									// Log level for bot console messages.
	PREFIX[MAX_CONSOLE_PREFIX_LENGTH],					// Console prefix for bot messages in console.
	bool:PRINT_EVENT_DATA 								// Enable/Disable printing event data in console (for debugging).
}