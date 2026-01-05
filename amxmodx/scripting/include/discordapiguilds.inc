#if defined _discordapiguilds_included
	#endinput
#endif
#define _discordapiguilds_included

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

/*
* Forwards
*
*/

/**
* Triggered after a bot has been added in a guild.
*
* @note JSON guild data
*{
*	"id": "80351110224678913",
*	"name": "Guild 2"
*}
*
* @param identifier         Bot identifier.
*/
forward OnGuildCreated(const identifier[], const guild_data[]);

/**
* Triggered after a bot has been removed from a guild.
*
* @note JSON guild data
*{
*	"id": "80351110224678913",
*	"name": "Guild 2"
*	"unavailable": false
*}
*
* @param identifier         Bot identifier.
*/
forward OnGuildDeleted(const identifier[], const guild_data[]);

/*
*
* Natives
*
*/

/**
* Get guilds where the bot it's added. Data it's in JSON format (array of objects).
*
* @note JSON buffer data on success:
*[
*  { "id": "80351110224678912", "name": "Guild 1" },
*  { "id": "80351110224678913", "name": "Guild 2" }
*]
*
* @note Buffer size must be at least 2. If bot it's not added in any guild,
*		buffer will be an empty JSON array. 
*
* @param identifier         Bot identifier.
* @param buffer             Buffer where guilds JSON will be copied.
* @param buffer_len			Buffer size.
*
* @return 0 if bot does not exists or it's not ready, number of guilds otherwise.
* @return 0 if buffer size is not > 2.
*/
native GetGuilds(const identifier[], buffer[], const buffer_len);