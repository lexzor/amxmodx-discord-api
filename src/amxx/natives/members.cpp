#include "members.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"
#include "amx_natives_helpers.h"

cell AMX_NATIVE_CALL GuildMemberExistsById(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* userIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake userId = dpp::snowflake(userIdentifier);

	return static_cast<cell>(guild->members.count(userId) > 0);
}

cell AMX_NATIVE_CALL GuildMemberExistsByUsername(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* username = MF_GetAmxString(amx, params[3], 2, nullptr);

	for (const auto& [userId, member] : guild->members)
	{
		const dpp::user* user = dpp::find_user(userId);

		if (user != nullptr && user->username == username)
		{
			return TRUE;
		}
	}

	return FALSE;
}

cell AMX_NATIVE_CALL GetGuildMemberIdByUsername(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* username = MF_GetAmxString(amx, params[3], 2, nullptr);

	const dpp::user* found = nullptr;

	for (const auto& [userId, member] : guild->members)
	{
		const dpp::user* user = dpp::find_user(userId);

		if (user != nullptr && user->username == username)
		{
			found = user;
			break;
		}
	}

	if (found == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Member with username '%s' not found in guild '%s'", __func__, username, guildIdentifier);
		return FALSE;
	}

	const std::string idStr = found->id.str();

	cell* buffer = MF_GetAmxAddr(amx, params[4]);
	const cell bufferLen = params[5];

	if (bufferLen < 0)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Buffer length cannot be negative", __func__);
		return FALSE;
	}

	if (idStr.size() > static_cast<size_t>(bufferLen))
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Buffer too small", __func__);
		return FALSE;
	}

	for (size_t i = 0; i < idStr.size(); i++)
	{
		buffer[i] = static_cast<cell>(idStr[i]);
	}

	buffer[idStr.size()] = 0x0;

	return TRUE;
}

cell AMX_NATIVE_CALL GetGuildMemberStringMemberById(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	AMX_GET_MEMBER(3, 2, FALSE)

	enum class MemberMemberString : uint32_t { NICKNAME, USERNAME, DISPLAY_NAME };

	std::string value;

	switch (static_cast<MemberMemberString>(params[4]))
	{
	case MemberMemberString::NICKNAME:
		value = member->get_nickname();
		break;

	case MemberMemberString::USERNAME:
	{
		const dpp::user* user = dpp::find_user(memberId);
		if (user == nullptr)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "(%s) User %s not found in cache", __func__, memberIdentifier);
			return FALSE;
		}
		value = user->username;
		break;
	}

	case MemberMemberString::DISPLAY_NAME:
		value = member->get_nickname().empty()
			? (dpp::find_user(memberId) ? dpp::find_user(memberId)->global_name : "")
			: member->get_nickname();
		break;

	default:
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid MemberMemberString value %i", __func__, params[4]);
		return FALSE;
	}

	cell* buffer = MF_GetAmxAddr(amx, params[5]);
	const cell bufferLen = params[6];

	if (bufferLen < 0)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Buffer length cannot be negative", __func__);
		return FALSE;
	}

	if (value.size() > static_cast<size_t>(bufferLen))
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Buffer too small", __func__);
		return FALSE;
	}

	for (size_t i = 0; i < value.size(); i++)
	{
		buffer[i] = static_cast<cell>(value[i]);
	}

	buffer[value.size()] = 0x0;

	return TRUE;
}

cell AMX_NATIVE_CALL GetGuildMemberIntegerMemberById(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	AMX_GET_MEMBER(3, 2, FALSE)

	enum class MemberIntegerMember : uint32_t { JOINED_AT, COMMUNICATION_DISABLED_UNTIL, PREMIUM_SINCE, IS_DEAF, IS_MUTED, IS_PENDING };

	switch (static_cast<MemberIntegerMember>(params[4]))
	{
	case MemberIntegerMember::JOINED_AT:
		return static_cast<cell>(member->joined_at);

	case MemberIntegerMember::COMMUNICATION_DISABLED_UNTIL:
		return static_cast<cell>(member->communication_disabled_until);

	case MemberIntegerMember::PREMIUM_SINCE:
		return static_cast<cell>(member->premium_since);

	case MemberIntegerMember::IS_DEAF:
		return static_cast<cell>(member->is_deaf());

	case MemberIntegerMember::IS_MUTED:
		return static_cast<cell>(member->is_muted());

	case MemberIntegerMember::IS_PENDING:
		return static_cast<cell>(member->is_pending());

	default:
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid MemberIntegerMember value %i", __func__, params[4]);
		return FALSE;
	}
}

cell AMX_NATIVE_CALL GetGuildMembersCount(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	return static_cast<cell>(guild->members.size());
}

cell AMX_NATIVE_CALL GetGuildMemberStringMemberByIndex(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const cell index = params[3];

	if (index < 0 || static_cast<size_t>(index) >= guild->members.size())
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Member index %i out of bounds", __func__, index);
		return FALSE;
	}

	const dpp::guild_member* member = nullptr;
	dpp::snowflake memberId;
	size_t currentIndex = 0;

	for (const auto& [id, m] : guild->members)
	{
		if (currentIndex++ != static_cast<size_t>(index))
			continue;

		member = &m;
		memberId = id;
		break;
	}

	if (member == nullptr)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Member index %i out of bounds", __func__, index);
		return FALSE;
	}

	enum class MemberStringMember : uint32_t { NICKNAME, USERNAME, DISPLAY_NAME };

	std::string value;

	switch (static_cast<MemberStringMember>(params[4]))
	{
	case MemberStringMember::NICKNAME:
		value = member->get_nickname();
		break;

	case MemberStringMember::USERNAME:
	{
		const dpp::user* user = dpp::find_user(memberId);
		if (user == nullptr)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "(%s) User %s not found in cache", __func__, memberId.str().c_str());
			return FALSE;
		}
		value = user->username;
		break;
	}

	case MemberStringMember::DISPLAY_NAME:
	{
		const dpp::user* user = dpp::find_user(memberId);
		const std::string nickname = member->get_nickname();
		value = !nickname.empty() ? nickname : (user != nullptr ? user->global_name : "");
		break;
	}

	default:
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid MemberStringMember value %i", __func__, params[4]);
		return FALSE;
	}

	cell* buffer = MF_GetAmxAddr(amx, params[5]);
	const cell bufferLen = params[6];

	if (bufferLen < 0)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Buffer length cannot be negative", __func__);
		return FALSE;
	}

	if (value.size() > static_cast<size_t>(bufferLen))
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Buffer too small", __func__);
		return FALSE;
	}

	for (size_t i = 0; i < value.size(); i++)
	{
		buffer[i] = static_cast<cell>(value[i]);
	}

	buffer[value.size()] = 0x0;

	return TRUE;
}

cell AMX_NATIVE_CALL GetGuildMemberIntegerMemberByIndex(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const cell index = params[3];

	if (index < 0 || static_cast<size_t>(index) >= guild->members.size())
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Member index %i out of bounds", __func__, index);
		return FALSE;
	}

	const dpp::guild_member* member = nullptr;
	size_t currentIndex = 0;

	for (const auto& [id, m] : guild->members)
	{
		if (currentIndex++ != static_cast<size_t>(index))
			continue;

		member = &m;
		break;
	}

	if (member == nullptr)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Member index %i out of bounds", __func__, index);
		return FALSE;
	}

	enum class MemberIntegerMember : uint32_t { JOINED_AT, COMMUNICATION_DISABLED_UNTIL, PREMIUM_SINCE, IS_DEAF, IS_MUTED, IS_PENDING };

	switch (static_cast<MemberIntegerMember>(params[4]))
	{
	case MemberIntegerMember::JOINED_AT:
		return static_cast<cell>(member->joined_at);

	case MemberIntegerMember::COMMUNICATION_DISABLED_UNTIL:
		return static_cast<cell>(member->communication_disabled_until);

	case MemberIntegerMember::PREMIUM_SINCE:
		return static_cast<cell>(member->premium_since);

	case MemberIntegerMember::IS_DEAF:
		return static_cast<cell>(member->is_deaf());

	case MemberIntegerMember::IS_MUTED:
		return static_cast<cell>(member->is_muted());

	case MemberIntegerMember::IS_PENDING:
		return static_cast<cell>(member->is_pending());

	default:
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid MemberIntegerMember value %i", __func__, params[4]);
		return FALSE;
	}
}

cell AMX_NATIVE_CALL GuildMemberHasRole(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	AMX_GET_MEMBER(3, 2, FALSE)

	const char* roleIdentifier = MF_GetAmxString(amx, params[4], 3, nullptr);
	const dpp::snowflake roleId = dpp::snowflake(roleIdentifier);

	return static_cast<cell>(std::find(member->get_roles().begin(), member->get_roles().end(), roleId) != member->get_roles().end());
}

cell AMX_NATIVE_CALL AddGuildMemberRole(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	AMX_GET_MEMBER(3, 2, FALSE)

	const char* roleIdentifier = MF_GetAmxString(amx, params[4], 3, nullptr);
	const dpp::snowflake roleId = dpp::snowflake(roleIdentifier);

	if (std::find(guild->roles.begin(), guild->roles.end(), roleId) == guild->roles.end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Role '%s' not found in guild '%s'", __func__, roleIdentifier, guildIdentifier);
		return FALSE;
	}

	bot->GetCluster().guild_member_add_role(guildId, memberId, roleId, [bot, guildId, memberId, roleId](const dpp::confirmation_callback_t& cb) {
		if (cb.is_error())
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;
			
			g_EventsQueue->Push([bot, guildId, memberId, roleId, errorCode, errorMessage, humanReadable]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to add role %s to member %s. Code: %u", bot->GetIdentifier().c_str(), roleId.str().c_str(), memberId.str().c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());

				ExecuteForward(ON_GUILD_MEMBER_ROLE_ADD, bot->GetIdentifier().c_str(), guildId.str().c_str(), memberId.str().c_str(), roleId.str().c_str(), false);
				return;
			});
		}
		else 
		{
			g_EventsQueue->Push([bot, guildId, memberId, roleId]() {
				ExecuteForward(ON_GUILD_MEMBER_ROLE_ADD, bot->GetIdentifier().c_str(), guildId.str().c_str(), memberId.str().c_str(), roleId.str().c_str(), true);
			});
		}
	});

	return TRUE;
}

cell AMX_NATIVE_CALL RemoveGuildMemberRole(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	AMX_GET_MEMBER(3, 2, FALSE)

	const char* roleIdentifier = MF_GetAmxString(amx, params[4], 3, nullptr);
	const dpp::snowflake roleId = dpp::snowflake(roleIdentifier);

	if (std::find(member->get_roles().begin(), member->get_roles().end(), roleId) == member->get_roles().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Member '%s' does not have role '%s'", __func__, memberIdentifier, roleIdentifier);
		return FALSE;
	}

	bot->GetCluster().guild_member_remove_role(guildId, memberId, roleId, [bot, guildId, memberId, roleId](const dpp::confirmation_callback_t& cb) {
			if (cb.is_error())
			{
				const uint32_t errorCode = cb.get_error().code;
				const std::string errorMessage = cb.get_error().message;
				const std::string humanReadable = cb.get_error().human_readable;
				
				g_EventsQueue->Push([bot, guildId, memberId, roleId, errorCode, errorMessage, humanReadable]() {
					gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to add role %s to member %s. Code: %u", bot->GetIdentifier().c_str(), roleId.str().c_str(), memberId.str().c_str(), errorCode);
					gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
					gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());

					ExecuteForward(ON_GUILD_MEMBER_ROLE_REMOVE, bot->GetIdentifier().c_str(), guildId.str().c_str(), memberId.str().c_str(), roleId.str().c_str(), false);
					return;
				});
			}
			else 
			{
				g_EventsQueue->Push([bot, guildId, memberId, roleId]() {
					ExecuteForward(ON_GUILD_MEMBER_ROLE_REMOVE, bot->GetIdentifier().c_str(), guildId.str().c_str(), memberId.str().c_str(), roleId.str().c_str(), true);
				});
			}
		});

	return TRUE;
}

cell AMX_NATIVE_CALL FetchGuildUserById(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const char* userIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake userId(userIdentifier);
	const std::string guildIdStr = guildId.str();

	bot->GetCluster().guild_get_member(guildId, userId, [bot, guildIdStr, userId](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::guild_member member = cb.get<dpp::guild_member>();
			const dpp::user* user = dpp::find_user(userId);

			const std::string userIdStr = userId.str();
			const std::string username = user != nullptr ? user->username : "";
			const std::string nickname = member.get_nickname();
			const std::string displayName = !nickname.empty() ? nickname : (user != nullptr ? user->global_name : "");

			g_EventsQueue->Push([bot, guildIdStr, userIdStr, username, nickname, displayName]() {
				ExecuteForward(ON_GUILD_MEMBER_FETCH, bot->GetIdentifier().c_str(), guildIdStr.c_str(), true, userIdStr.c_str(), username.c_str(), nickname.c_str(), displayName.c_str());
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;
			const std::string userIdStr = userId.str();

			g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, guildIdStr, userIdStr]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to fetch guild member %s. Code: %u", bot->GetIdentifier().c_str(), userIdStr.c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());

				ExecuteForward(ON_GUILD_MEMBER_FETCH, bot->GetIdentifier().c_str(), guildIdStr.c_str(), false, userIdStr.c_str(), "", "", "");
			});
		}
		});

	return TRUE;
}

cell AMX_NATIVE_CALL FetchGuildUserByUsername(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const char* username = MF_GetAmxString(amx, params[3], 2, nullptr);
	const std::string usernameQuery(username);

	if (usernameQuery.empty())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Username cannot be empty", __func__);
		return FALSE;
	}

	const std::string guildIdStr = guildId.str();

	auto searchPage = std::make_shared<std::function<void(dpp::snowflake)>>();

	// this may be slower
	*searchPage = [bot, guildId, guildIdStr, usernameQuery, searchPage](dpp::snowflake after) {
		bot->GetCluster().guild_get_members(guildId, 1000, after,
			[bot, guildId, guildIdStr, usernameQuery, searchPage](const dpp::confirmation_callback_t& cb) {
				if (cb.is_error())
				{
					const uint32_t errorCode = cb.get_error().code;
					const std::string errorMessage = cb.get_error().message;
					const std::string humanReadable = cb.get_error().human_readable;

					g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, guildIdStr, usernameQuery]() {
						gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to list guild members while searching for %s. Code: %u", bot->GetIdentifier().c_str(), usernameQuery.c_str(), errorCode);
						gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
						gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());

						ExecuteForward(ON_GUILD_MEMBER_FETCH, bot->GetIdentifier().c_str(), guildIdStr.c_str(), false, "", usernameQuery.c_str(), "", "");
					});
					return;
				}

				const dpp::guild_member_map members = cb.get<dpp::guild_member_map>();

				dpp::snowflake lastId = 0;

				for (const auto& [userId, member] : members)
				{
					lastId = userId;

					const dpp::user* user = dpp::find_user(userId);

					if (user == nullptr || user->username != usernameQuery)
						continue;

					const std::string userIdStr = userId.str();
					const std::string foundUsername = user->username;
					const std::string nickname = member.get_nickname();
					const std::string displayName = !nickname.empty() ? nickname : user->global_name;

					g_EventsQueue->Push([bot, guildIdStr, userIdStr, foundUsername, nickname, displayName]() {
						ExecuteForward(ON_GUILD_MEMBER_FETCH, bot->GetIdentifier().c_str(), guildIdStr.c_str(), true, userIdStr.c_str(), foundUsername.c_str(), nickname.c_str(), displayName.c_str());
					});
					return;
				}

				// Fewer than 1000 returned means this was the last page, list exhausted, no match
				if (members.size() < 1000)
				{
					g_EventsQueue->Push([bot, guildIdStr, usernameQuery]() {
						ExecuteForward(ON_GUILD_MEMBER_FETCH, bot->GetIdentifier().c_str(), guildIdStr.c_str(), false, "", usernameQuery.c_str(), "", "");
					});

					return;
				}

				// Full page returned. More members may exist, fetch the next page
				(*searchPage)(lastId);
			});
		};

	(*searchPage)(0);

	return TRUE;
}