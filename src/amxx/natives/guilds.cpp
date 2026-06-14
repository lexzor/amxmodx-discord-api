#include "guilds.h"

cell AMX_NATIVE_CALL GetGuilds(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GetGuilds) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
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
			{ "id", guild.id.str() },
			{ "name", guild.name }
		});
	}

	const std::string guildsJSON = jGuilds.dump();

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

	return static_cast<cell>(guilds.size());
}

cell AMX_NATIVE_CALL GuildChannelExists(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);
	
	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);
	
	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExists) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}
	
	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExists) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}
	
	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	
	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(dpp::snowflake(guildIdentifier)); 
	
	if(guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExists) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}
	
	const char* channelIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	
	const std::vector<dpp::snowflake>& guildChannels = guildsMapIt->second.channels;
	const dpp::snowflake requestedChannel(channelIdentifier);
	
	for(const dpp::snowflake& channel : guildChannels)
	{
		if(channel == requestedChannel)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

cell AMX_NATIVE_CALL GetGuildChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);
	
	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);
	
	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GetGuildChannel) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}
	
	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GetGuildChannel) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);

	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(guildId); 
	
	if(guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GetGuildChannel) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}
	
	const char* channelIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::channel* channel = dpp::find_channel(dpp::snowflake(channelIdentifier));

	if(channel->guild_id != guildId)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExists) Bot %s channel %s exists, but not in guild %s (current channel guild is %s)", identifier, channelIdentifier, guildIdentifier, channel->guild_id.str().c_str());
		return FALSE;
	}

	cell* channelNameBuffer = MF_GetAmxAddr(amx, params[4]);
	int channelNameBufferLen = params[5];

	if(channel->name.size() > channelNameBufferLen)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(GuildChannelExists) Bot %s index out of bounds for name buffer", identifier);
		return FALSE;
	}

	int currentLen = 0;
	while (channelNameBufferLen-- && currentLen < channel->name.size())
	{
		*channelNameBuffer++ = static_cast<cell>(channel->name.at(currentLen));
		currentLen++;
	}
	*channelNameBuffer = 0x0;

	cell* channelParentIdBuffer = MF_GetAmxAddr(amx, params[6]);
	int channelParentIdBufferLen = params[7];

	if(channel->name.size() > channelParentIdBufferLen)
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(GuildChannelExists) Bot %s index out of bounds for parent id buffer", identifier);
		return FALSE;
	}

	currentLen = 0;
	while (channelParentIdBufferLen-- && currentLen < channel->parent_id.str().size())
	{
		*channelParentIdBuffer++ = static_cast<cell>(channel->parent_id.str().at(currentLen));
		currentLen++;
	}
	*channelParentIdBuffer = 0x0;

	return TRUE;
}