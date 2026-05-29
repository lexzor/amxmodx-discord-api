/*
* Discord API Module for AMX Mod X. Below you will find all the natives and forwards provided by the module. 
*
* Author: lexzor
* Website: https://github.com/lexzor/amxmodx-discord-api
*
*/

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#if defined _discordapiutils_included
	#endinput
#endif
#define _discordapiutils_included

#include <amxmodx>
#include <amxmisc>
#include <discordapi>

/**
* Checks if the plugin API have same version as module.
*
* @note If versions does not correspond a log it's printed to console.
*
* @return True if version matches or false otherwise 
*/
stock bool:CheckAPIVersion(const bool:log_error)
{
    new buffer[16]
    GetModuleVersionString(buffer, charsmax(buffer)); 

    new bool:same = bool:equal(DISCORD_API_AMXX_API_VERSION_STRING, buffer);

    if(!same && log_error)
        log_amx("AMXX API version %s does not correspond with module version %s", DISCORD_API_AMXX_API_VERSION_STRING, buffer);

    return same;
}