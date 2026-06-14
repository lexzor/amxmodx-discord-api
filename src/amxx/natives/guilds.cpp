#include "guilds.h"
#include "amxx/pending_amx_object_store_impl.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"

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

cell AMX_NATIVE_CALL BeginCreateGuildChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildChannel) Bot with identifier '%s' does not exists", identifier);
		return -1;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildChannel) Bot with identifier '%s' is not ready", identifier);
		return -1;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);

	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(guildId);

	if (guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildChannel) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return -1;
	}

	PendingAmxObjectStoreHandle handle = g_PendingAmxObjectStore->CreateObject<dpp::channel>();
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(handle);

	channel->set_guild_id(guildId);

	return handle;
}

cell AMX_NATIVE_CALL SetGuildChannelMemberString(AMX* amx, cell* params)
{
	cell channelHandle = params[1];
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(channelHandle);

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SetGuildChannelMemberString) Invalid channel handle %i", channelHandle);
		return FALSE;
	}

	cell stringMemberType = params[2];
	const char* buffer = MF_GetAmxString(amx, params[3], 2, nullptr);

	enum class ChannelMemberString : uint32_t
	{
		NAME,
		PARENT_ID
	};

	switch (static_cast<ChannelMemberString>(stringMemberType))
	{
	case ChannelMemberString::NAME:
		channel->set_name(buffer);
		break;

	case ChannelMemberString::PARENT_ID:
		channel->set_parent_id(dpp::snowflake(buffer));
		break;
	}

	return TRUE;
}

cell AMX_NATIVE_CALL EndCreateGuildChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildChannel) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildChannel) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	cell channelHandle = params[2];
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(channelHandle);

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildChannel) Invalid channel handle %i", channelHandle);
		return FALSE;
	}

	bot->GetCluster().channel_create(*channel, [bot, channelHandle](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const std::string channelId = cb.get<dpp::channel>().id.str();
			g_EventsQueue->Push([bot, channelId, channelHandle]() {
				ExecuteForward(ON_GUILD_CHANNEL_CREATE, bot->GetIdentifier().c_str(), channelHandle, true, channelId.c_str());
			});
		}
		else
		{
			uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelHandle]() {
				ExecuteForward(ON_GUILD_CHANNEL_CREATE, bot->GetIdentifier().c_str(), channelHandle, false, "");

				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create Discord channel. Code: %s", bot->GetIdentifier().c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
			});
		}
	});

	g_PendingAmxObjectStore->RemoveObject(channelHandle);

	return TRUE;
}

cell AMX_NATIVE_CALL DeleteGuildChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildChannel) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildChannel) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);

	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(guildId);

	if (guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildChannel) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}

	const char* channelIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const std::string channelId(channelIdentifier);


	bot->GetCluster().channel_delete(dpp::snowflake(channelIdentifier), [bot, channelId](const dpp::confirmation_callback_t& cb) {
		
		if (!cb.is_error())
		{
			g_EventsQueue->Push([bot, channelId]() {
				ExecuteForward(ON_GUILD_CHANNEL_DELETE, bot->GetIdentifier().c_str(), true, channelId.c_str());
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelId]() {

				ExecuteForward(ON_GUILD_CHANNEL_DELETE, bot->GetIdentifier().c_str(), false, channelId.c_str());

				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to delete Discord channel %s. Code: %i", bot->GetIdentifier().c_str(), channelId.c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
			});
		}
	});

	return TRUE;
}