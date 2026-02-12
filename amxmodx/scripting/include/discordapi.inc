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

#include <discordapiconsts>
#include <discordapibot>
#include <discordapibotchannels>
#include <discordapibotguilds>

/**
* Get the module version string.
*
* @param buffer         	Buffer to store the version string.
* @param len              	Length of the buffer.
*
*/
native GetModuleVersionString(const buffer[], const len);