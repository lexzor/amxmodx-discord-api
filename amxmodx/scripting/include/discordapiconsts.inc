#if defined _discordapiconsts_included
	#endinput
#endif
#define _discordapiconsts_included

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#define AMXX_API_VERSION_STRING "1.0.0-BETA"

const MAX_IDENTIFIER_LENGTH			= 32;
const MAX_CONSOLE_PREFIX_LENGTH 	= 32;

enum LogLevel
{
	NONE,
	DEFAULT,
	VERBOSE
}

enum Options
{
	LogLevel:LOG_LEVEL,									// Log level for bot console messages.
	PREFIX[MAX_CONSOLE_PREFIX_LENGTH],					// Console prefix for bot messages in console.
	bool:PRINT_EVENT_DATA 								// Enable/Disable printing event data in console (for debugging).
}