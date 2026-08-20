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

#if defined _discordapibotguilds_included
	#endinput
#endif
#define _discordapibotguilds_included

#include <discordapibotguildsconsts>

/*
* Forwards
*
*/

/**
* Triggered after a bot has been added in a guild.
*
* @param identifier         Bot identifier.
* @param guild_id			Created guild ID.
* @param guild_name			Created guild name.
*/
forward OnGuildCreated(const identifier[], const guild_id[], const guild_name[]);

/**
* Triggered after a bot has been removed from a guild or a guild became unavailable.
*
* @param identifier         Bot identifier.
* @param guild				Deleted guild ID.
* @param guild_name			Deleted guild name.
* @param unavailable		Bot removed or guild unavailable.
*/
forward OnGuildDeleted(const identifier[], const guild_id[], const guild_name[], const bool:unavailable);

/**
* Triggered after a channel create response has been received.
*
* @param identifier         			Bot identifier.
* @param channel_handle					Channel handle returned by BeginCreateGuildChannel or INVALID_CHANNEL_HANDLE
* 										if the channel has not been created by a request sent by EndCreateGuildChannel.
* @param success						True if channel has been created successfully, false otherwise.
* @param channel_id						Channel id if the channel has been created successfully, empty otherwise.
*/
forward OnGuildChannelCreate(const identifier[], const ChannelHandle:channel_handle, const bool:success, const channel_id[]);

/**
* Triggered when a channel delete response has been received.
*
* @param identifier         			Bot identifier.
* @param success						True if channel has been deleted successfully, false otherwise.
* @param channel_id						Channel ID.
*/
forward OnGuildChannelDelete(const identifier[], const bool:success, const channel_id[]);

/**
* Triggered after a channel edit response has been received.
*
* @param identifier         			Bot identifier.
* @param channel_handle					Channel handle returned by BeginCreateGuildChannel or INVALID_CHANNEL_HANDLE
* 										if the channel has not been created by a request sent by EndCreateGuildChannel
* @param success						True if channel has been edited successfully, false otherwise.
* @param channel_id						Channel ID.
*/
forward OnGuildChannelEdit(const identifier[], const ChannelHandle:channel_handle, const bool:success, const channel_id[]);

/**
* Triggered after a guild slash command create response has been received.
*
* @param identifier         			Bot identifier.
* @param success						True if channel has been created successfully, false otherwise.
* @param name							Name of the guild slash command.
* @param slash_command_id				ID of guild slash command (empty string on fail)
*/
forward OnGuildSlashCommandCreate(const identifier[], const bool:success, const name[], const slash_command_id[]);

/**
* Triggered after a guild slash command delete response has been received.
*
* @param identifier         			Bot identifier.
* @param success						True if channel has been created successfully, false otherwise.
* @param name							Name of the guild slash command.
* @param slash_command_id				ID of guild slash command (empty string on fail)
*/
forward OnGuildSlashCommandDelete(const identifier[], const bool:success, const name[], const slash_command_id[]);

/**
* Triggered after a guild slash command create response has been received.
*
* @note This command it's an interaction.
*
* @param identifier         			Bot identifier.
* @param name							Name of the command.
* @param caller							Discord user id who called this guild slash command.
* @param options						Values of guild slash command options.
* @param channel_id						Channel ID where the user called this guild slash command.
*/
forward OnGuildSlashCommand(const identifier[], const guild_id[], const name[], const caller_id[], const caller_name[], const options[], const channel_id[]);

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
* @param identifier         			Bot identifier.
* @param buffer             			Buffer where guilds JSON will be copied.
* @param buffer_len						Buffer size.
*
* @return 0 if bot does not exists or it's not ready, number of guilds otherwise.
* @return 0 if buffer size is not > 2.
*/
native any:GetGuilds(const identifier[], buffer[], const buffer_len);

/**
* Get the number of cached channels from a guild.
*
* @param identifier                     Bot identifier.
* @param guild_id						Guild ID to check.
*
* @return Number of cached channels or 0 if bot does not exists, it's not ready,
*		  guild is invalid or guild is not cached.
*/
native GetGuildChannelsCount(const identifier[], const guild_id[]);

/**
* Copy a string member from a guild channel found by index.
*
* @note Index is zero-based and follows the guild channels cache order.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
* @param index                          Channel index in guild cache.
* @param member                         Channel string member to copy, from ChannelMemberString.
* @param buffer                         Buffer where member value will be copied.
* @param buffer_len                     Buffer max index, usually passed with charsmax().
*
* @return True if channel has been found and member has been copied, false otherwise.
*/
native GetGuildChannelStringMemberByIndex(const identifier[], const guild_id[], index, ChannelMemberString:member, buffer[], buffer_len);

/**
* Get an integer member from a guild channel found by index.
*
* @note Index is zero-based and follows the guild channels cache order.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
* @param index                          Channel index in guild cache.
* @param member                         Channel integer member to get, from ChannelMemberInt.
*
* @return Requested integer value or 0 if bot does not exists, it's not ready, guild is invalid, channel is invalid or member is invalid.
*/
native GetGuildChannelIntegerMemberByIndex(const identifier[], const guild_id[], index, ChannelMemberInt:member);

/**
* Copy a string member from a guild channel found by ID.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
* @param channel_id                     Channel ID to search for.
* @param member                         Channel string member to copy, from ChannelMemberString.
* @param buffer                         Buffer where member value will be copied.
* @param buffer_len                     Buffer max index, usually passed with charsmax().
*
* @return True if channel has been found in the guild and member has been copied, false otherwise.
*/
native GetGuildChannelStringMemberById(const identifier[], const guild_id[], const channel_id[], ChannelMemberString:member, buffer[], buffer_len);

/**
* Get an integer member from a guild channel found by ID.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
* @param channel_id                     Channel ID to search for.
* @param member                         Channel integer member to get, from ChannelMemberInt.
*
* @return Requested integer value or 0 if bot does not exists, it's not ready, guild is invalid, channel is invalid, channel does not belong to the guild, or member is invalid.
*/
native GetGuildChannelIntegerMemberById(const identifier[], const guild_id[], const channel_id[], ChannelMemberInt:member);

/**
* Check if a channel exists in a guild searching by channel ID.
*
* @param identifier         			Bot identifier.
* @param guild_id						Guild ID to check.
* @param channel_id						Channel ID to search for.
*
* @return								True if bot it's in specified guild and channel has been found, false otherwise.					
*/
native bool:GuildChannelExistsById(const identifier[], const guild_id[], const channel_id[]);

/**
* Check if a channel exists in a guild searching by channel name.
*
* @param identifier         			Bot identifier.
* @param guild_id						Guild ID to check.
* @param channel_id						Channel name to search for.
*
* @return								Number of occurences if bot it's in specified guild and more
*										than 1 channel has been found, -1 otherwise.					
*/
native GuildChannelExistsByName(const identifier[], const guild_id[], const channel_name[]);

/**
 * Begins creating a new channel.
 *
 * @param identifier    				Bot identifier.
 * @param guild_id      				Guild ID in which the channel will be created.
 *
 * @return              				A ChannelHandle on success if bot it's added in the guild, or INVALID_CHANNEL_HANDLE on failure.
 */
native ChannelHandle:BeginCreateGuildChannel(const identifier[], const guild_id[]);

/**
 * Finalizes channel creation and sends the request to the Discord API.
 *
 * @note A response will be sent in OnGuildChannelCreate forward.
 * @note The channel_handle passed here will later be passed to OnGuildChannelCreate.
 *
 * @param identifier     				Bot identifier.
 * @param channel_handle 				Channel handle returned by BeginCreateGuildChannel.
 *
 * @return               				True if the request was successfully sent to the Discord API, false otherwise.
 */
native bool:EndCreateGuildChannel(const identifier[], ChannelHandle:channel_handle);

/**
 * Send a request to Discord API to delete a specific channel.
 *
 * @note A response will be sent in OnGuildChannelDelete forward.
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
native bool:SetGuildChannelMemberString(ChannelHandle:channel_handle, ChannelMemberString:member, const buffer[]);

/**
 * Sets an integer member on a channel that is being created.
 *
 * @param channel_handle 				Channel handle returned by BeginCreateGuildChannel.
 * @param member         				The member to set, from ChannelMemberInt.
 * @param value         				The value to assign to the member.
 *
 * @return               				True if the member was successfully set, false if channel_handle is invalid.
 */
native bool:SetGuildChannelMemberInteger(ChannelHandle:channel_handle, ChannelMemberInt:member, { ChannelType, ChannelFlag, _}:value);

/**
 * Begins editing an existing channel.
 *
 * @param identifier    				Bot identifier.
 * @param channel_id      				Guild ID in which the channel will be created.
 *
 * @return              				A ChannelHandle on success if bot it's added in the guild, or INVALID_CHANNEL_HANDLE on failure.
 */
native ChannelHandle:BeginEditGuildChannel(const identifier[], const channel_id[]);

/**
 * Finalizes channel editing and sends the request to the Discord API.
 *
 * @note A response will be sent in OnGuildChannelCreate forward.
 * @note The channel_handle passed here will later be passed to OnGuildChannelCreate forward.
 *
 * @param identifier     				Bot identifier.
 * @param channel_handle 				Channel handle returned by BeginC reateGuildChannel.
 *
 * @return               				True if the request was successfully sent to the Discord API, false otherwise.
 */
native bool:EndEditGuildChannel(const identifier[], ChannelHandle:channel_handle);

/**
 * Check if a guild slash command exists and copy it's ID in a buffer.
 *
 * @note Could return false if the bot did not fetched yet the slash commands from guild.
 *
 * @param identifier     				Bot identifier.
 * @param guild_id 						Guild ID where to search for the guild slash command.
 * @param name							Name of the guild slash command, withour prefix "/".
 * @param slash_command_id_buffer		Buffer to copy the ID of the guild slash command.
 * @param slash_command_id_buffer_len	Buffer length for guild slash command ID.
 *
 * @return               				True if the bot's added in the guild and slash_command_buffer 
 *										buffer length it's big enough.
 */
native bool:GuildSlashCommandExists(const identifier[], const guild_id[], const name[], slash_command_id_buffer[] = "", slash_command_buffer_id_len = 0);

/**
 * Begins creating a new guild slash command.
 *
 * @param identifier    				Bot identifier.
 * @param guild_id      				Guild ID in which the channel will be created.
 * @param name			      			Name of the guild slash command to create, without prefix "/".
 * @param description					Description of the guild .slash command
 *
 * @return              				A SlashCommandHandle on success if slash command does not exists, or INVALID_SLASH_COMMAND_HANDLE on failure.
 */
native SlashCommandHandle:BeginCreateGuildSlashCommand(const identifier[], const name[], const description[] = "");

/**
 * Finalizes guild slash command creation and sends the request to the Discord API.
 *
 * @note A response will be sent in OnGuildSlashCommandCreate forward.
 * @note The slash_command_handle value passed here will later be passed to OnGuildSlashCommandCreate forward.
 *
 * @param identifier     				Bot identifier.
 * @param guild_id						Guild ID where the guild slash command should be created.
 * @param slash_command_handle			Guild slash command handle returned by BeginCreateGuildSlashCommand.
 *
 * @return               				True if the request was successfully sent to the Discord API, false otherwise.
 */
native bool:EndCreateGuildSlashCommand(const identifier[], SlashCommandHandle:slash_command_handle, const guild_id[]);

/**
 * Add a slash command options to a guild slash command which it's beign created.
 *
 * @param slash_command_handle     		Guild slash command handle where option should be added, returned by BeginCreateGuildSlashCommand.
 * @param slash_command_option_type		Type of guild slash command option, from SlashCommandOptionType enum.
 * @param name							Name of the guild slash command option.
 * @param description					Description of the guild slash command option (optional).
 * @param required						It is required to send guild slash command to bot.
 *
 * @return               				True if guild slash command handle, slash command option type and name are valid,
 * 										false otherwise.
 */
native bool:AddSlashCommandOption(SlashCommandHandle:slash_command_handle, SlashCommandOptionType:slash_command_option_type, const name[], const description[] = "", const bool:required = false);

/**
 * Begins creating a guild slash command option.
 *
 * @note This native it's used only if you need to add choices to the option.
 *
 * @param slash_command_option_type		Type of slash command option, from SlashCommandOptionType enum.
 * @param name							Name of the guild slash command option.
 * @param description					Description of the guild slash command option (optional).
 * @param required						It is required to send guild slash command to bot.
 * @param slash_command_handle     		Guild slash command handle returned by BeginCreateGuildSlashCommand.
 *
 * @return               				True if guild slash command option type and name are valid, false otherwise.
 */
native SlashCommandOptionHandle:BeginCreateSlashCommandOption(SlashCommandOptionType:slash_command_option_type, const name[], const description[] = "", const bool:required = false);

/**
 * Finalizes guild slash command option creation and add it to the in creation guild slash command.
 *
 * @param slash_command_option_handle	Guild slash command option handle returned by BeginCreateGuildSlashCommandOption.
 * @param slash_command_handle			Guild slash command handle where option should be added, returned by BeginCreateGuildSlashCommand.
 *
 * @return               				True if slash command option type and name are valid, false otherwise.
 */
native bool:EndCreateSlashCommandOption(SlashCommandOptionHandle:slash_command_option_handle, SlashCommandHandle:slash_command_handle);

/**
 * Add a integer choice to a slash command option.
 *
 * @param slash_command_option_handle	Slash command option handle returned by BeginCreateGuildSlashCommandOption.
 * @param name							Name of the slash command option choice.
 * @param value							Integer value for the slash command option choice.
 *
 * @return               				True if slash command option handle and name are valid, false otherwise.
 */
native bool:AddSlashCommandOptionChoiceInteger(SlashCommandOptionHandle:slash_command_option_handle, const name[], const value);

/**
 * Add a string choice to a slash command option.
 *
 * @param slash_command_option_handle	Slash command option handle returned by BeginCreateGuildSlashCommandOption.
 * @param name							Name of the slash command option choice.
 * @param buffer						String value for the slash command option choice.
 *
 * @return               				True if slash command option handle and name are valid, false otherwise.
 */
native bool:AddSlashCommandOptionChoiceString(SlashCommandOptionHandle:slash_command_option_handle, const name[], const buffer[]);

/**
 * Add a float choice to a slash command option.
 *
 * @note Passed float value will be automatically converted to a double value.
 *
 * @param slash_command_option_handle	Slash command option handle returned by BeginCreateGuildSlashCommandOption.
 * @param name							Name of the slash command option choice.
 * @param value							Float value for the slash command option choice.
 *
 * @return               				True if slash command option handle and name are valid, false otherwise.
 */
native bool:AddSlashCommandOptionChoiceFloat(SlashCommandOptionHandle:slash_command_option_handle, const name[], const Float:value);

/**
 * Delete a guild slash command from Discord API.
 *
 * @note Use this native to create a guild slash commands which does not contain any options or
 *		 options choice.
 * @note Could return false if the bot did not fetched yet the slash commands from guild.
 *
 * @param identifier					Bot identifier.
 * @param guild_id						Guild ID to delete the slash command from.
 * @param name							Name of the guild slash command to create, without prefix "/".
 * @param description					Description of the guild slash command (optional).
 *
 * @return               				True if bot it's in specified guild and request
 *										has been successfully sent to Discord API, false otherwise.
 */
native bool:CreateGuildSlashCommand(const identifier[], const guild_id[], const name[], const description[] = "");

/**
 * Delete a guild slash command from Discord API.
 *
 * @note Could return false if the bot did not fetched yet the guild slash commands from guild.
 *
 * @param identifier					Bot identifier.
 * @param guild_id						Guild ID to delete the slash command from.
 * @param slash_command_id				Guild slash command ID to be deleted.
 *
 * @return               				True if bot it's in specified guild and request
 *										has been successfully sent to Discord API, false otherwise.
 */
native bool:DeleteGuildSlashCommand(const identifier[], const guild_id[], const slash_command_id[]);
