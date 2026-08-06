/*
* Discord API Module for AMX Mod X. Below you will find all the natives,forwards and constants provided by the module. 
*
* Author: lexzor
* Website: https://github.com/lexzor/amxmodx-discord-api
*
*/

#pragma reqlib discordapi
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib discordapi
#endif

#if defined _discordapibotguildsconsts_included
	#endinput
#endif
#define _discordapibotguildsconsts_included

/**
*
* Enums
*
*/

// Used in SetGuildChannelMemberString native as second parameter.
enum ChannelMemberString
{
	/**
	 * @note The name of the channel.
	 */
	NAME,

	/**
	 * @note Parent of the channel.
	 */
	PARENT_ID,
}

// Used in SetGuildChannelMemberInt native as second parameter.
enum ChannelMemberInt
{
	/**
	 * @note Type of the channel, from ChannelType enum.
	 */
	TYPE,

	/**
	 * @note Flags of the channel, from ChannelFlag enum.
	 */
	FLAGS,

	/**
	 * @note User limit in a voice channel. Between 0 and 99.
	 */
	USER_LIMIT
}

// Used in SetGuildChannelMemberInt third parameter.
enum ChannelFlag
{
	/**
	 * @note NSFW Gated Channel.
	 */
	NSFW                     = (1 << 4),  // 0b0000000000010000

	/**
	 * @note Video quality forced to 720p.
	 */
	VIDEO_QUALITY_720P       = (1 << 5),  // 0b0000000000100000

	/**
	 * @note Lock permissions (only used when updating channel positions).
	 */
	LOCK_PERMISSIONS         = (1 << 6),  // 0b0000000001000000

	/**
	 * @note Thread is pinned to the top of its parent forum or media channel.
	 */
	PINNED_THREAD            = (1 << 7),  // 0b0000000010000000

	/**
	 * @note Whether a tag is required to be specified when creating a thread in a forum or media channel.
	 * Tags are specified in the thread's applied_tags field.
	 */
	REQUIRE_TAG              = (1 << 8),  // 0b0000000100000000

	/**
	 * @note When set hides the embedded media download options. Available only for media channels.
	 */
	HIDE_MEDIA_DOWNLOAD_OPTIONS = (1 << 12)  // 0b0001000000000000
}

/**
 * Used in SetGuildChannelMemberInt third parameter. 
 * Below channel type values are copied from DPP (v10.1.5)
 */
enum ChannelType
{
	/**
	 * @note A text channel within a server.
	 */
	CHANNEL_TEXT = 0,

	/**
	 * @note A direct message between users.
	 */
	DM = 1,

	/**
	 * @note A voice channel within a server.
	 */
	CHANNEL_VOICE = 2,

	/**
	 * @note a direct message between multiple users
	 * @deprecated this channel type was intended to be used with the now deprecated GameBridge SDK.
	 * Existing group dms with bots will continue to function, but newly created channels will be unusable.
	 */
	GROUP_DM = 3,

	/**
	 * @note An organizational category that contains up to 50 channels.
	 */
	CHANNEL_CATEGORY = 4,

	/**
	 * @note A channel that users can follow and cross-post into their own server.
	 */
	CHANNEL_ANNOUNCEMENT = 5,

	/**
	 * @note A channel in which game developers can sell their game on Discord.
	 * @deprecated Store channels are deprecated by Discord.
	 */
	CHANNEL_STORE = 6,

	/**
	 * @note A temporary sub-channel within a `GUILD_ANNOUNCEMENT` channel.
	 */
	CHANNEL_ANNOUNCEMENT_THREAD = 10,

	/**
	 * @note A temporary sub-channel within a `GUILD_TEXT` or `GUILD_FORUM` channel.
	 */
	CHANNEL_PUBLIC_THREAD = 11,

	/**
	 * @note A temporary sub-channel within a `GUILD_TEXT` channel
	 * that is only viewable by those invited and those with the `MANAGE_THREADS` permission.
	 */
	CHANNEL_PRIVATE_THREAD = 12,

	/**
	 * @note A "stage" channel, like a voice channel with one authorised speaker.
	 */
	CHANNEL_STAGE = 13,

	/**
	 * @note The channel in a hub containing the listed servers.
	 *
	 * @see https://support.discord.com/hc/en-us/articles/4406046651927-Discord-Student-Hubs-FAQ
	 */
	CHANNEL_DIRECTORY = 14,

	/**
	 * @note Forum channel that can only contain threads.
	 */
	CHANNEL_FORUM = 15,

	/**
	 * @note Media channel that can only contain threads, similar to forum channels.
	 */
	CHANNEL_MEDIA = 16,
}

/**
 * Used in AddSlashCommandOption second parameter. 
 * Below slash command options values are copied from DPP (v10.1.5)
 */
enum SlashCommandOptionType {
	/**
	 * @note A sub-command.
	 */
	SUB_COMMAND = 1,

	/**
	 * @note A sub-command group.
	 */
	SUB_COMMAND_GROUP = 2,

	/**
	 * @note A string value.
	 */
	STRING = 3,

	/**
	 * @note An integer value.
	 */
	INTEGER = 4,

	/**
	 * @note A boolean value.
	 */
	BOOLEAN = 5,

	/**
	 * @note A user snowflake id.
	 */
	USER = 6,

	/**
	 * @note A channel snowflake id. Includes all channel types and categories.
	 */
	CHANNEL = 7,

	/**
	 * @note A role id (snowflake).
	 */
	ROLE = 8,

	/**
	 * @note A mentionable (users and roles).
	 */
	MENTIONABLE = 9,

	/**
	 * @note Any double between -2^53 and 2^53.
	 */
	NUMBER = 10,

	/**
	 * @note File attachment type.
	 */
	ATTACHMENT = 11,
};

/*
* Constants
*
*/

/**
 * Invalid channel handle value, returned by BeginCreateGuildChannel and BeginEditGuildChannel
 * in case of failing to create a ChannelHandle
 */
const ChannelHandle:INVALID_CHANNEL_HANDLE = ChannelHandle:-1;

/**
 * Invalid channel handle value, returned by BeginCreateGuildSlashCommand and BeginCreateGuildSlashCommand
 * in case of failing to create a SlashCommandHandle
 */
const SlashCommandHandle:INVALID_SLASH_COMMAND_HANDLE = SlashCommandHandle:-1;