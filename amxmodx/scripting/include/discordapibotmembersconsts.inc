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

#if defined _discordapibotmembersconsts_included
	#endinput
#endif
#define _discordapibotmembersconsts_included


// Used in GetGuildMemberStringMemberByIndex and GetGuildMemberStringMember natives.
enum MemberStringMember
{
	/**
	 * @note The member's nickname in the guild.
	 */
	DAPI_MEMBER_NICKNAME = 0,

	/**
	 * @note The member's username.
	 */
	DAPI_MEMBER_USERNAME,

	/**
	 * @note The member's display name.
	 */
	DAPI_MEMBER_DISPLAY_NAME
};

// Used in GetGuildMemberIntegerMemberByIndex and GetGuildMemberIntegerMemberById natives.
enum MemberIntegerMember
{
	/**
	 * @note Unix timestamp of when the member joined the guild.
	 */
	DAPI_MEMBER_JOINED_AT = 0,

	/**
	 * @note Unix timestamp until which the member is timed out, or 0 if not timed out.
	 * @note Discord does not always clear this value once the timeout expires;
	 *		 compare against the current time rather than treating a non-zero
	 *		 value as "still timed out".
	 */
	DAPI_MEMBER_COMMUNICATION_DISABLED_UNTIL,

	/**
	 * @note Unix timestamp of when the member started boosting the guild, or 0 if not boosting.
	 */
	DAPI_MEMBER_PREMIUM_SINCE,

	/**
	 * @note 1 if the member is deafened in voice channels, 0 otherwise.
	 */
	DAPI_MEMBER_IS_DEAF,

	/**
	 * @note 1 if the member is muted in voice channels, 0 otherwise.
	 */
	DAPI_MEMBER_IS_MUTED,

	/**
	 * @note 1 if the member has not yet completed membership screening, 0 otherwise.
	 */
	DAPI_MEMBER_IS_PENDING
}