#if defined _discordapichannels_included
	#endinput
#endif
#define _discordapichannels_included

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

/*
*
* Forwards
*
*/

/**
* Triggered when a message is created.
*
* @note This event is an interaction.
*
* @note Event JSON data:
* {
*     "id": "80351110224678912",
*     "content": "Hello world from Discord!",
*     "guild_id": "302050872383242240",
*     "author": {
*         "id": "80351110224678912",
*         "username": "lexzor"
*     },
*     "mentions": [{ author id and username strings }...],
* }
*
* @param identifier         Bot identifier.
* @param channel_id         Channel structure where the message was created.
* @param event_data         Event JSON data string.
*/
forward OnChannelMessageCreated(const identifier[], const channel_id[], const event_data[]);

/*
*
* Natives
*
*/

/**
* Send a message to a channel.
*
* @param identifier         Bot identifier.
* @param channel_id         Channel ID.
* @param message			Text message to send.
*
* @return False if bot does not exists or is not ready, true otherwise.
*/
native bool:SendMessageToChannel(const identifier[], const channel_id[], const message[]);