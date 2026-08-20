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

#if defined _discordapibotroles_included
	#endinput
#endif

#define _discordapibotroles_included


/*
*
* Natives
*
*/


/**
* Check if a role exists in a guild by role ID.
*
* @param identifier						Bot identifier.
* @param guild_id           			Guild ID to check.
* @param role_id            			Role ID to search for.
*
* @return True if the role has been found in the guild cache, false otherwise.
*/
native bool:GuildRoleExistsById(const identifier[], const guild_id[], const role_id[]);

/**
* Check if a role exists in a guild by role name.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to check.
* @param role_name           			Role name to search for.
*
* @return True if the role has been found in the guild cache, false otherwise.
*/
native bool:GuildRoleExistsByName(const identifier[], const guild_id[], const role_name[]);

/**
* Get a guild role's ID by role name.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to search in.
* @param role_name          			Role name to search for.
* @param output             			Buffer where the role ID will be copied.
* @param len                			Buffer size.
*
* @return True if the role has been found and the role ID has been copied, false otherwise.
*/
native bool:GetGuildRoleIdByName(const identifier[], const guild_id[], const role_name[], output[], len);