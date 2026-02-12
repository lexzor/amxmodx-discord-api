#if defined _discordapiutils_included
	#endinput
#endif
#define _discordapiutils_included

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#include <amxmodx>
#include <amxmisc>
#include <discordapiconsts>

/**
* Checks if the plugin API have same version as module.
*
* @note If versions does not correspond a log it's printed to console.
*
* @return True if version matches or false otherwise 
*/
stock boolean:CheckAPIVersion()
{
    new buffer[16]
    GetModuleVersionString(buffer, charsmax(buffer));

    if(!equal(buffer, DISCORD_API_AMXX_API_VERSION_STRING))
    {
        log_amx("[DiscordAPI] AMXX API version %s does not correspond with module version %s", DISCORD_API_AMXX_API_VERSION_STRING, buffer);
        return false;
    }

    return true;
}