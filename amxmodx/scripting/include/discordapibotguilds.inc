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

#if defined _discordapibotguilds_included
	#endinput
#endif
#define _discordapibotguilds_included

/*
* Enums
*
*/

// Used in SetGuildChannelMemberString native as second parameter
enum ChannelMemberString
{
	/**
	 * @note The name of the channel
	 */
	NAME,

	/**
	 * @note Parent of the channel
	 */
	PARENT_ID,
}

// Used in SetGuildChannelMemberInt native as second parameter
enum ChannelMemberTypeInt
{
	TYPE
}

// Used in 
enum ChannelFlag
{
	/**
	 * @note NSFW Gated Channel
	 */
	NSFW                     = (1 << 0),

	/**
	 * @note Video quality forced to 720p
	 */
	VIDEO_QUALITY_720P       = (1 << 1),

	/**
	 * @note Lock permissions (only used when updating channel positions)
	 */
	LOCK_PERMISSIONS         = (1 << 2),

	/**
	 * @note Thread is pinned to the top of its parent forum or media channel
	 */
	PINNED_THREAD            = (1 << 3),

	/**
	 * @note Whether a tag is required to be specified when creating a thread in a forum or media channel.
	 * Tags are specified in the thread's applied_tags field.
	 */
	REQUIRE_TAG              = (1 << 4),

	/**
	 * @note When set hides the embedded media download options. Available only for media channels
	 */
	HIDE_MEDIA_DOWNLOAD_OPTIONS = (1 << 5)
}

// Below channel type values are copied from DPP (v10.1.5)
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

// enum ChannelMemberIntAcceptedIntValue { ChannelType, ChannelFlag }

/*
* Constants
*
*/

// Invalid channel handle value returned by BeginCreateGuildChannel
const ChannelCreateHandle:INVALID_CHANNEL_HANDLE = ChannelCreateHandle:-1;

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

/**
* Triggered when a channel has been created.
*
* @param identifier         Bot identifier.
* @param channel_handle		Channel handle returned by BeginCreateGuildChannel 
*							or INVALID_CHANNEL_HANDLE if the channel has not been created by a request sent by EndCreateGuildChannel.
* @param success			True if channel has been created successfully, false otherwise
* @param channel_id			Channel id if the channel has been created successfully, empty otherwise
*/
forward OnGuildChannelCreate(const identifier[], const ChannelCreateHandle:channel_handle, const bool:success, const channel_id[]);

/**
* Triggered when a channel has been deleted.
*
* @param identifier         Bot identifier.
* @param channel_id			Channel ID.
* @param success			True if channel has been deleted successfully, false otherwise
*/
forward OnGuildChannelDelete(const identifier[], const bool:success, const channel_id[]);

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
* @note An error will be thrown if the buffer length is too small for the
*		returned data from Discord API.
*
* @param identifier         Bot identifier.
* @param buffer             Buffer where guilds JSON will be copied.
* @param buffer_len			Buffer size.
*
* @return 0 if bot does not exists or it's not ready, number of guilds otherwise.
* @return 0 if buffer size is not > 2.
*/
native any:GetGuilds(const identifier[], buffer[], const buffer_len);

/**
* Check if a channel exists in a guild searching by channel ID.
*
* @param identifier         			Bot identifier.
* @param guild_id						Guild ID to check.
* @param channel_id						Channel ID to search for.
*
* @return								True if bot it's in guild and channel has been found, false otherwise.					
*/
native bool:GuildChannelExists(const identifier[], const guild_id[], const channel_id[]);

/**
* Get a guild's channel name and parent id.
*
* @param identifier         			Bot identifier.
* @param guild_id						Guild ID to check.
* @param channel_id						Channel ID to search for.
* @param name_buffer					Buffer to copy channel's name in.
* @param name_buffer_len				Buffer size for name.
* @param parent_id_buffer				Buffer to copy channel's parent id in.
* @param parent_id_buffer_len			Buffer size for channel's parent id.
*
* @return								True if bot it's in guild and channel has been found, false otherwise.					
*/
native bool:GetGuildChannel(const identifier[], const guild_id[], const channel_id[], name_buffer[], name_buffer_len, parent_id_buffer[], parent_id_buffer_len);

/**
 * Begins creating a new channel.
 *
 * @param identifier    				Bot identifier.
 * @param guild_id      				Guild ID in which the channel will be created.
 *
 * @return              				A ChannelCreateHandle on success, or INVALID_CHANNEL_HANDLE on failure.
 */
native ChannelCreateHandle:BeginCreateGuildChannel(const identifier[], const guild_id[]);

/**
 * Finalizes channel creation and sends the request to the Discord API.
 *
 * @note The channel_handle passed here will later be passed to OnChannelCreateEvent.
 *
 * @param identifier     				Bot identifier.
 * @param channel_handle 				Channel handle returned by BeginCreateGuildChannel.
 *
 * @return               				True if the request was successfully sent to the Discord API, false otherwise.
 */
native bool:EndCreateGuildChannel(const identifier[], ChannelCreateHandle:channel_handle);

/**
 * Send a request to Discord API to delete a specific channel
 *
 * @param channel_handle 				Channel handle returned by BeginCreateGuildChannel.
 * @param member         				The member to set, from ChannelMemberString.
 * @param buffer         				The value to assign to the member.
 *
 * @return               				True if , false if channel_handle is invalid.
 */
native bool:DeleteGuildChannel(const identifier[], const guild_id[], const channel_id[]);

/**
 * Sets a string member on a channel that is being created.
 *
 * @param channel_handle 				Channel handle returned by BeginCreateGuildChannel.
 * @param member         				The member to set, from ChannelMemberString.
 * @param buffer         				The value to assign to the member.
 *
 * @return               				True if the member was successfully set, false if channel_handle is invalid.
 */
native bool:SetGuildChannelMemberString(ChannelCreateHandle:channel_handle, ChannelMemberString:member, const buffer[]);

// native bool:SetGuildChannelMemberInt(ChannelCreateHandle:channel_handle, ChannelMemberInt:member, ChannelMemberIntAcceptedValues:value);