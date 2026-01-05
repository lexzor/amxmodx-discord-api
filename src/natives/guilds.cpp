#include "guilds.h"
#include "common.h"

cell AMX_NATIVE_CALL GetGuilds(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GetGuilds) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->GetReadyState())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GetGuilds) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	int bufferLen = params[3];
	cell* destination = MF_GetAmxAddr(amx, params[2]);
	
	if (bufferLen < 2)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s (GetGuilds) Buffer length must be at least 2", bot->GetConsolePrefix().c_str());
		return FALSE;
	}
	
	memset(destination, 0x0, bufferLen);

	DiscordBot::GuildsMap guilds = bot->GetGuildsMap();

	if (!guilds.size())
	{
		destination[0] = static_cast<cell>('[');
		destination[1] = static_cast<cell>(']');

		return FALSE;
	}

	dpp::json jGuilds = dpp::json::array();

	for (const auto& [snowflake, guild] : guilds)
	{
		jGuilds.push_back({
			{ "id", guild.id.str()},
			{ "name", guild.name}
		});
	}

	std::string guildsJSON = jGuilds.dump();

	if (guildsJSON.size() > bufferLen)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "%s (GetGuilds) Buffer length too small. Current guilds JSON size: %i", bot->GetConsolePrefix().c_str(), guildsJSON.size());
		return FALSE;
	}

	int currentLen = 0;
	while (bufferLen-- && currentLen < guildsJSON.size())
	{
		*destination++ = static_cast<cell>(guildsJSON.at(currentLen));
		currentLen++;
	}

	*destination = 0x0;

	return guilds.size();
}