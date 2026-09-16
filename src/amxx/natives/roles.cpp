#include "roles.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"
#include "amx_natives_helpers.h"

cell AMX_NATIVE_CALL GuildRoleExistsById(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* roleIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake roleId = dpp::snowflake(roleIdentifier);

	return static_cast<cell>(std::find(guild->roles.begin(), guild->roles.end(), roleId) != guild->roles.end());
}

cell AMX_NATIVE_CALL GuildRoleExistsByName(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* roleName = MF_GetAmxString(amx, params[3], 2, nullptr);

	for (const dpp::snowflake& roleId : guild->roles)
	{
		const dpp::role* role = dpp::find_role(roleId);

		if (role != nullptr && role->name == roleName)
		{
			return TRUE;
		}
	}

	return FALSE;
}

cell AMX_NATIVE_CALL GetGuildRoleIdByName(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* roleName = MF_GetAmxString(amx, params[3], 2, nullptr);

	const dpp::role* found = nullptr;

	for (const dpp::snowflake& roleId : guild->roles)
	{
		const dpp::role* role = dpp::find_role(roleId);

		if (role != nullptr && role->name == roleName)
		{
			found = role;
			break;
		}
	}

	if (found == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Role '%s' not found in guild '%s'", __func__, roleName, guildIdentifier);
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