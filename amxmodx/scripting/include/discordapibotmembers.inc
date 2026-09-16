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

#if defined _discordapibotmembers_included
	#endinput
#endif

#define _discordapibotmembers_included

#include <discordapibotmembersconsts>


/*
*
* Forwards
*
*/

/**
* Triggered when a member joins a guild.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID the member joined.
* @param user_id            			Discord user ID of the new member.
* @param nickname           			Guild nickname of the new member, empty if none set.
* @param username           			Username of the new member.
*/
forward OnGuildMemberCreate(const identifier[], const guild_id[], const user_id[], const nickname[], const username[]);

/**
* Triggered when a member leaves or is removed from a guild.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID the member left.
* @param user_id            			Discord user ID of the removed member.
* @param username           			Username of the removed member.
*/
forward OnGuildMemberDelete(const identifier[], const guild_id[], const user_id[], const username[]);

/**
* Triggered when a guild member's data changes (nickname, roles, timeout, etc).
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID where the member was updated.
* @param user_id            			Discord user ID of the updated member.
*/
forward OnGuildMemberUpdate(const identifier[], const guild_id[], const user_id[]);

/**
* Triggered after a guild member fetch (by ID or username) response has been received.
*
* @param identifier      				Bot identifier.
* @param guild_id        				Guild ID the member was fetched from.
* @param success         				True if the member was found, false otherwise.
* @param user_id         				User ID of the fetched member, empty string on failure.
* @param username        				Username of the fetched member, empty string on failure.
* @param nickname        				Guild nickname of the fetched member, empty string if none set or on failure.
* @param display_name    				Nickname if set, otherwise global display name; empty string on failure.
*/
forward OnGuildMemberFetch(const identifier[], const guild_id[], const bool:success, const user_id[], const username[], const nickname[], const display_name[]);

/**
* Triggered after a guild member role add request has been sent.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID.
* @param user_id            			Discord user ID.
* @param role_id            			Role ID.
* @param success            			True if the role has been added successfully, false otherwise.
*/
forward OnGuildMemberRoleAdd(const identifier[], const guild_id[], const user_id[], const role_id[], const bool:success);

/**
* Triggered after a guild member role remove has been sent.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID.
* @param user_id            			Discord user ID.
* @param role_id            			Role ID.
* @param success            			True if the role has been added successfully, false otherwise.
*/
forward OnGuildMemberRoleRemove(const identifier[], const guild_id[], const user_id[], const role_id[], const bool:success);

/*
*
* Natives
*
*/

/**
* Check if a guild member exists in the cache by user ID.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to check.
* @param user_id            			User ID to search for.
*
* @return True if the guild member has been found in the cache, false otherwise.
*/
native bool:GuildMemberExistsById(const identifier[], const guild_id[], const user_id[]);

/**
* Check if a guild member exists in the cache by username.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to check.
* @param username           			Username to search for.
*
* @return True if the guild member has been found in the cache, false otherwise.
*/
native bool:GuildMemberExistsByUsername(const identifier[], const guild_id[], const username[]);

/**
* Get a guild member's user ID by username.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to search in.
* @param username           			Username to search for.
* @param output             			Buffer where the user ID will be copied.
* @param len                			Buffer size.
*
* @return True if the guild member has been found and the user ID has been copied, false otherwise.
*/
native bool:GetGuildMemberIdByUsername(const identifier[], const guild_id[], const username[], output[], const len);

/**
* Get a string member value from a guild member searchinb by Id.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to search in.
* @param user_id            			User ID of the guild member.
* @param member             			Guild member string member to get, from MemberMemberString.
* @param output             			Buffer where the member value will be copied.
* @param len                			Buffer size.
*
* @return True if the guild member has been found and the requested value has been copied, false otherwise.
*/
native bool:GetGuildMemberStringMemberById(const identifier[], const guild_id[], const user_id[], MemberStringMember:member, output[], const len);

/**
* Get an integer member from a guild member found by ID.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to search in.
* @param user_id                        User ID of the guild member.
* @param member                         Member integer member to get, from MemberIntegerMember.
*
* @return Requested integer value or 0 if bot does not exists, it's not ready, guild is invalid, member is invalid or member value is invalid.
*/
native GetGuildMemberIntegerMemberById(const identifier[], const guild_id[], const user_id[], MemberIntegerMember:member);

/**
* Get the number of cached members from a guild.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
*
* @return Number of cached members or 0 if bot does not exists, it's not ready,
*		  guild is invalid or guild is not cached.
*/
native GetGuildMembersCount(const identifier[], const guild_id[]);

/**
* Copy a string member from a guild member found by index.
*
* @note Index is zero-based and follows the guild members cache order, which
*		is not guaranteed to match join order or any other meaningful ordering.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
* @param index                          Member index in guild cache.
* @param member                         Member string member to copy, from MemberStringMember.
* @param buffer                         Buffer where member value will be copied.
* @param buffer_len                     Buffer max index, usually passed with charsmax().
*
* @return True if member has been found and value has been copied, false otherwise.
*/
native GetGuildMemberStringMemberByIndex(const identifier[], const guild_id[], index, MemberStringMember:member, buffer[], buffer_len);

/**
* Get an integer member from a guild member found by index.
*
* @note Index is zero-based and follows the guild members cache order, which
*		is not guaranteed to match join order or any other meaningful ordering.
*
* @param identifier                     Bot identifier.
* @param guild_id                       Guild ID to check.
* @param index                          Member index in guild cache.
* @param member                         Member integer member to get, from MemberIntegerMember.
*
* @return Requested integer value or 0 if bot does not exists, it's not ready, guild is invalid, member is invalid or member value is invalid.
*/
native GetGuildMemberIntegerMemberByIndex(const identifier[], const guild_id[], index, MemberIntegerMember:member);

/**
* Check if a guild member has a specific role.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to search in.
* @param user_id            			User ID of the guild member.
* @param role_id            			Role ID to check.
*
* @return True if the guild member has the specified role, false otherwise.
*/
native bool:GuildMemberHasRole(const identifier[], const guild_id[], const user_id[], const role_id[]);

/**
* Add a role to a guild member through the Discord API.
*
* @note A response will be sent in the OnGuildMemberRoleAdd forward.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID where the member belongs.
* @param user_id            			User ID of the guild member.
* @param role_id            			Role ID to add.
*
* @return True if the request was successfully sent to the Discord API, false otherwise.
*/
native bool:AddGuildMemberRole(const identifier[], const guild_id[], const user_id[], const role_id[]);

/**
* Remove a role from a guild member through the Discord API.
*
* @note A response will be sent in the OnGuildMemberRoleRemove forward.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID where the member belongs.
* @param user_id            			User ID of the guild member.
* @param role_id            			Role ID to remove.
*
* @return True if the request was successfully sent to the Discord API, false otherwise.
*/
native bool:RemoveGuildMemberRole(const identifier[], const guild_id[], const user_id[], const role_id[]);

/**
* Fetch a guild member from the Discord API by user ID, bypassing the local cache.
*
* @note This is an asynchronous request. The result will be delivered via the
*		OnGuildMemberFetched forward.
* @note Unlike GuildMemberExistsById, this always hits the Discord API and will
*		find members not yet present in the local guild member cache.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to fetch the member from.
* @param user_id            			User ID of the member to fetch.
*
* @return True if the request was successfully sent to the Discord API, false otherwise.
*/
native bool:FetchGuildUserById(const identifier[], const guild_id[], const user_id[]);

/**
* Fetch a guild member from the Discord API by exact username, bypassing the local cache.
*
* @note It's recommended to always use FetchGuildUserById, this may be slower 
* 		based on number of members.
*
* @note This is an asynchronous request. The result will be delivered via the
*		OnGuildMemberFetched forward.
* @note Internally performs a prefix search against guild members and filters for an
*		exact username match; if the guild has more members matching the searched prefix
*		than the search can return in a single page, an existing exact match may not be found.
*
* @param identifier         			Bot identifier.
* @param guild_id           			Guild ID to search in.
* @param username            			Exact username to search for.
*
* @return True if the request was successfully sent to the Discord API, false otherwise.
*/
native bool:FetchGuildUserByUsername(const identifier[], const guild_id[], const username[]);