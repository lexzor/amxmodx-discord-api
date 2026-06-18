#include "guilds.h"
#include "amxx/pending_amx_object_store_impl.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"
#include "utils/strtolower.h"

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

cell AMX_NATIVE_CALL GuildChannelExistsById(AMX* amx, cell* params)
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

cell AMX_NATIVE_CALL GuildChannelExistsByName(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExistsByName) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExistsByName) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);

	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(dpp::snowflake(guildIdentifier));

	if (guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildChannelExistsByName) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}

	const char* channelName = MF_GetAmxString(amx, params[3], 2, nullptr);

	for (const dpp::snowflake channelId : guildsMapIt->second.channels)
	{
		const dpp::channel* channel = dpp::find_channel(channelId);

		if (channel && channel->name == std::string_view(channelName))
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
			const dpp::channel createdChannel = cb.get<dpp::channel>();
			const std::string channelId = createdChannel.id.str();

			auto& guildsMap = bot->GetGuildsMap();
			auto it = guildsMap.find(createdChannel.guild_id);
				
			if (it != guildsMap.end())
				it->second.channels.push_back(createdChannel.id);

			dpp::channel* cached = new dpp::channel(createdChannel);
			dpp::get_channel_cache()->store(cached);

			g_EventsQueue->Push([bot, channelId, channelHandle]() {
				ExecuteForward(ON_GUILD_CHANNEL_CREATE, bot->GetIdentifier().c_str(), channelHandle, true, channelId.c_str());
				g_PendingAmxObjectStore->RemoveObject(channelHandle);
			});
		}
		else
		{
			uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelHandle, humanReadable]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create Discord channel. Code: %s", bot->GetIdentifier().c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());
				
				ExecuteForward(ON_GUILD_CHANNEL_CREATE, bot->GetIdentifier().c_str(), channelHandle, false, "");
				g_PendingAmxObjectStore->RemoveObject(channelHandle);
			});
		}

	});

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

	bot->GetCluster().channel_delete(channelId, [bot, channelId, guildId](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			auto& guildsMap = bot->GetGuildsMap();
			auto it = guildsMap.find(guildId);
			
			const dpp::snowflake channelSnowflakeId = dpp::snowflake(channelId);
			
			if (it != guildsMap.end())
			{
				auto& channels = it->second.channels;
				channels.erase(
					std::remove(channels.begin(), channels.end(), channelSnowflakeId),
					channels.end()
				);
			}

			dpp::get_channel_cache()->remove(dpp::find_channel(channelSnowflakeId));
			
			g_EventsQueue->Push([bot, channelId]() {
				ExecuteForward(ON_GUILD_CHANNEL_DELETE, bot->GetIdentifier().c_str(), true, channelId.c_str());
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelId, humanReadable]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to delete Discord channel %s. Code: %i", bot->GetIdentifier().c_str(), channelId.c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());

				ExecuteForward(ON_GUILD_CHANNEL_DELETE, bot->GetIdentifier().c_str(), false, channelId.c_str());
			});
		}
	});

	return TRUE;
}

cell AMX_NATIVE_CALL SetGuildChannelMemberString(AMX* amx, cell* params)
{
	const char* buffer = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(buffer))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SetGuildChannelMemberString) Buffer can't be empty");
		return FALSE;
	}

	cell channelHandle = params[1];
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(channelHandle);

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SetGuildChannelMemberString) Invalid channel handle %i", channelHandle);
		return FALSE;
	}

	cell stringMemberType = params[2];

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

cell AMX_NATIVE_CALL SetGuildChannelMemberInteger(AMX* amx, cell* params)
{
	cell channelHandle = params[1];
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(channelHandle);

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SetGuildChannelMemberInt) Invalid channel handle %i", channelHandle);
		return FALSE;
	}

	cell intMemberType = params[2];
	cell value = params[3];

	enum class ChannelMemberInt : uint32_t
	{
		TYPE,
		FLAGS,
		USER_LIMIT
	};

	switch (static_cast<ChannelMemberInt>(intMemberType))
	{
	case ChannelMemberInt::TYPE:
	{
		channel->set_type(static_cast<dpp::channel_type>(value));
		break;
	}

	case ChannelMemberInt::FLAGS:
	{
		auto applyFlag = [channel, value](const dpp::channel_flags flag) {
			(value & static_cast<cell>(flag)) ? channel->add_flag(flag) : channel->remove_flag(flag);
		};

		applyFlag(dpp::c_nsfw);
		applyFlag(dpp::c_video_quality_720p);
		applyFlag(dpp::c_lock_permissions);
		applyFlag(dpp::c_pinned_thread);
		applyFlag(dpp::c_require_tag);
		applyFlag(dpp::c_hide_media_download_options);
		break;
	}

	case ChannelMemberInt::USER_LIMIT:
	{
		if (value < 0 || value > 99)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "(SetGuildChannelMemberInt) Invalid USER_LIMIT member value %i. Minimum value is 0, maximum value is 99", value);
			return FALSE;
		}

		channel->set_user_limit(value);
		break;
	}
	}

	return TRUE;
}

cell AMX_NATIVE_CALL BeginEditGuildChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginEditGuildChannel) Bot with identifier '%s' does not exists", identifier);
		return -1;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginEditGuildChannel) Bot with identifier '%s' is not ready", identifier);
		return -1;
	}

	const char* channelIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	
	dpp::channel* channel = dpp::find_channel(dpp::snowflake(channelIdentifier));
	
	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginEditGuildChannel) Channel %s does not exists", channelIdentifier);
		return -1;
	}

	return g_PendingAmxObjectStore->CreateObject<dpp::channel>(*channel);
}

cell AMX_NATIVE_CALL EndEditGuildChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndEditGuildChannel) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndEditGuildChannel) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	cell channelHandle = params[2];
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(channelHandle);

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndEditGuildChannel) Invalid channel handle %i", channelHandle);
		return FALSE;
	}

	bot->GetCluster().channel_edit(*channel, [bot, channelHandle](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::channel updatedChannel = cb.get<dpp::channel>();
			const std::string channelId = updatedChannel.id.str();

			dpp::channel* cached = dpp::find_channel(updatedChannel.id);
			if (cached)
				*cached = updatedChannel;
			else
			{
				dpp::channel* newCached = new dpp::channel(updatedChannel);
				dpp::get_channel_cache()->store(newCached);
			}
			
			g_EventsQueue->Push([bot, channelId, channelHandle]() {
				ExecuteForward(ON_GUILD_CHANNEL_EDIT, bot->GetIdentifier().c_str(), channelHandle, true, channelId.c_str());
				g_PendingAmxObjectStore->RemoveObject(channelHandle);
			});
		}
		else
		{
			uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelHandle, humanReadable]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to edit Discord channel. Code: %s", bot->GetIdentifier().c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());

				ExecuteForward(ON_GUILD_CHANNEL_EDIT, bot->GetIdentifier().c_str(), channelHandle, false, "");
				g_PendingAmxObjectStore->RemoveObject(channelHandle);
			});
		}
	});

	return TRUE;
}

cell AMX_NATIVE_CALL GuildSlashCommandExists(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildSlashCommandExists) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildSlashCommandExists) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);
	const DiscordBot::GuildsMap::const_iterator botGuildsMapIt = bot->GetGuildsMap().find(guildId);

	if (botGuildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildSlashCommandExists) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}

	const char* slashCommandName = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(slashCommandName))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildSlashCommandExists) Invalid slash command passed", identifier, guildIdentifier);
		return FALSE;
	}

	DiscordBot::GuildSlashCommandsMap& guildsSlashCommandsMap = bot->GetGuildsSlashCommandsMap();
	const DiscordBot::GuildSlashCommandsMap::const_iterator guildsSlashCommandsMapIt = guildsSlashCommandsMap.find(guildId);

	if (guildsSlashCommandsMapIt == guildsSlashCommandsMap.end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(GuildSlashCommandExists) Guild's %s slash commands may not have been fetched yet from the Discord API for bot %s", guildIdentifier, bot->GetIdentifier().c_str());
		return FALSE;
	}

	cell* slashCommandBuffer = MF_GetAmxAddr(amx, params[4]);
	cell slashCommandBufferLen = params[5];

	for (const auto& [key, value] : guildsSlashCommandsMapIt->second)
	{
		if (value.name == std::string_view(slashCommandName))
		{
			if (slashCommandBufferLen == 0)
				return TRUE;

			const std::string slashCommandId = value.id.str();

			if (slashCommandId.size() > slashCommandBufferLen)
			{
				MF_LogError(amx, AMX_ERR_BOUNDS, "(GuildSlashCommandExists) Index out of bounds for slash command id buffer");
				return FALSE;
			}

			int currentLen = 0;
			while (slashCommandBufferLen-- && currentLen < slashCommandId.size())
			{
				*slashCommandBuffer++ = static_cast<cell>(slashCommandId.at(currentLen));
				currentLen++;
			}

			*slashCommandBuffer = 0x0;

			return TRUE;
		}
	}

	return FALSE;
}

cell AMX_NATIVE_CALL BeginCreateGuildSlashCommand(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return -1;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildSlashCommand) Bot with identifier '%s' is not ready", identifier);
		return -1;
	}

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildSlashCommand) Slash command must have a name");
		return -1;
	}

	const char* description = MF_GetAmxString(amx, params[3], 2, nullptr);

	return g_PendingAmxObjectStore->CreateObject<dpp::slashcommand>(strtolower(name), description, bot->GetCluster().me.id);
}

cell AMX_NATIVE_CALL EndCreateGuildSlashCommand(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildSlashCommand) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	cell slashCommandHandle = params[2];
	dpp::slashcommand* slashCommand = g_PendingAmxObjectStore->GetStoreObject<dpp::slashcommand>(slashCommandHandle);

	if (slashCommand == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildSlashCommand) Invalid guild slash command handle %i", slashCommandHandle);
		return FALSE;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);
	const DiscordBot::GuildsMap::const_iterator botGuildsMapIt = bot->GetGuildsMap().find(guildId);

	if (botGuildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateGuildSlashCommand) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}

	for (auto& opt : slashCommand->options)
	{
		gpMetaUtilFuncs->pfnLogConsole(
			PLID,
			"OPT name=%s type=%d choices=%zu",
			opt.name.c_str(),
			(int)opt.type,
			opt.choices.size()
		);
	}

	const std::string slashCommandName = slashCommand->name;

	bot->GetCluster().guild_command_create(*slashCommand, guildId, [bot, guildId, slashCommandName, slashCommandHandle](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::slashcommand slashCommand = cb.get<dpp::slashcommand>();
			g_EventsQueue->Push([bot, slashCommand, guildId, slashCommandHandle]() {
				bot->GetGuildsSlashCommandsMap()[guildId][slashCommand.id] = slashCommand;

				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), true, slashCommand.name.c_str(), slashCommand.id.str().c_str());
				g_PendingAmxObjectStore->RemoveObject(slashCommandHandle);
			});
		}
		else
		{
			uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, slashCommandName, slashCommandHandle]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create guild slash command %s. Code: %i", bot->GetIdentifier().c_str(), slashCommandName.c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());
				
				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), false, slashCommandName.c_str(), "");
				g_PendingAmxObjectStore->RemoveObject(slashCommandHandle);
			});
		}
	});

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOption(AMX* amx, cell* params)
{
	cell slashCommandHandle = params[1];
	cell slashCommandOptionType = params[2];

	if (slashCommandOptionType < 1 || slashCommandOptionType > 11)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOption) Invalid slash command option type %i", slashCommandOptionType);
		return FALSE;
	}

	const char* commandName = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(commandName))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOption) Slash command option must have a name");
		return FALSE;
	}

	const char* commandDescription = MF_GetAmxString(amx, params[4], 3, nullptr);
	cell slashCommandRequired = params[5];

	dpp::slashcommand* slashCommand = g_PendingAmxObjectStore->GetStoreObject<dpp::slashcommand>(slashCommandHandle);
	
	if (slashCommand == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOption) Invalid guild slash command handle %i", slashCommandHandle);
		return FALSE;
	}

	slashCommand->add_option(dpp::command_option(
		static_cast<dpp::command_option_type>(slashCommandOptionType),
		strtolower(commandName),
		commandDescription,
		static_cast<bool>(slashCommandRequired)
	));

	return TRUE;
}
cell AMX_NATIVE_CALL BeginCreateSlashCommandOption(AMX* amx, cell* params)
{
	cell slashCommandOptionType = params[1];

	if (slashCommandOptionType < 1 || slashCommandOptionType > 11)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOption) Invalid slash command option type %i", slashCommandOptionType);
		return -1;
	}

	const char* commandName = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(commandName))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOption) Slash command option must have a name");
		return -1;
	}

	const char* commandDescription = MF_GetAmxString(amx, params[3], 2, nullptr);
	cell slashCommandRequired = params[4];

	PendingAmxObjectStoreHandle handle = g_PendingAmxObjectStore->CreateObject<dpp::command_option>(
		static_cast<dpp::command_option_type>(slashCommandOptionType),
		strtolower(commandName),
		commandDescription,
		static_cast<bool>(slashCommandRequired)
	);

	return handle;
}

cell AMX_NATIVE_CALL EndCreateSlashCommandOption(AMX* amx, cell* params)
{
	const cell slashCommandOptionHandle = params[1];
	const cell slashCommandHandle = params[2];

	dpp::slashcommand* slashCommand = g_PendingAmxObjectStore->GetStoreObject<dpp::slashcommand>(slashCommandHandle);

	if (slashCommand == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateSlashCommandOption) Invalid slash command handle %i", slashCommandHandle);
		return FALSE;
	}

	dpp::command_option* slashCommandOption = g_PendingAmxObjectStore->GetStoreObject<dpp::command_option>(slashCommandOptionHandle);

	if (slashCommandOption == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(EndCreateSlashCommandOption) Invalid slash command option handle %i", slashCommandHandle);
		return FALSE;
	}

	slashCommand->add_option(*slashCommandOption);

	g_PendingAmxObjectStore->RemoveObject(slashCommandOptionHandle);

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceInteger(AMX* amx, cell* params)
{
	const cell slashCommandOptionHandle = params[1];

	dpp::command_option* slashCommandOption = g_PendingAmxObjectStore->GetStoreObject<dpp::command_option>(slashCommandOptionHandle);

	if (slashCommandOption == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Invalid slash command option handle %i", slashCommandOptionHandle);
		return FALSE;
	}

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Slash command option choice must have a name");
		return FALSE;
	}

	const cell value = params[3];

	slashCommandOption->add_choice(dpp::command_option_choice(strtolower(name), static_cast<int64_t>(value)));

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceString(AMX* amx, cell* params)
{
	const cell slashCommandOptionHandle = params[1];

	dpp::command_option* slashCommandOption = g_PendingAmxObjectStore->GetStoreObject<dpp::command_option>(slashCommandOptionHandle);

	if (slashCommandOption == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Invalid slash command option handle %i", slashCommandOptionHandle);
		return FALSE;
	}

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Slash command option choice must have a name");
		return FALSE;
	}

	const char* value = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(value))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Slash command option choice must have a value");
		return FALSE;
	}

	slashCommandOption->add_choice(dpp::command_option_choice(strtolower(name), std::string(value)));

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceFloat(AMX* amx, cell* params)
{
	const cell slashCommandOptionHandle = params[1];

	dpp::command_option* slashCommandOption = g_PendingAmxObjectStore->GetStoreObject<dpp::command_option>(slashCommandOptionHandle);

	if (slashCommandOption == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Invalid slash command option handle %i", slashCommandOptionHandle);
		return FALSE;
	}

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(AddSlashCommandOptionChoiceString) Slash command option choice must have a name");
		return FALSE;
	}

	const float value = amx_ctof(params[3]);

	slashCommandOption->add_choice(dpp::command_option_choice(strtolower(name), static_cast<double>(value)));

	return TRUE;
}

cell AMX_NATIVE_CALL CreateGuildSlashCommand(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Bot with identifier '%s' is not ready", identifier);
		return FALSE;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);
	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(guildId);

	if (guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}

	const DiscordBot::GuildSlashCommandsMap::const_iterator guildsSlashCommandsIt = bot->GetGuildsSlashCommandsMap().find(guildId);

	if (guildsSlashCommandsIt == bot->GetGuildsSlashCommandsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Could not found guild %s slash commands map for bot %s", guildIdentifier, identifier);
		return FALSE;
	}

	const char* name = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(BeginCreateGuildSlashCommand) Slash command must have a name");
		return FALSE;
	}

	const char* description = MF_GetAmxString(amx, params[4], 3, nullptr);

	const dpp::slashcommand slashCommand = dpp::slashcommand(strtolower(name), description, bot->GetCluster().me.id);

	bot->GetCluster().guild_command_create(slashCommand, guildId, [bot, guildId, name](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::slashcommand slashCommand = cb.get<dpp::slashcommand>();
			g_EventsQueue->Push([bot, slashCommand, guildId]() {
				bot->GetGuildsSlashCommandsMap()[guildId][slashCommand.id] = slashCommand;

				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), true, slashCommand.name.c_str(), slashCommand.id.str().c_str());
			});
		}
		else
		{
			uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, name]() {
				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), false, name, "");

				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create guild slash command %s. Code: %i", bot->GetIdentifier().c_str(), name, errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());
			});
		}
	});

	return TRUE;
}

cell AMX_NATIVE_CALL DeleteGuildSlashCommand(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return -1;
	}

	if (!bot->IsStarted())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Bot with identifier '%s' is not ready", identifier);
		return -1;
	}

	const char* guildIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);
	const DiscordBot::GuildsMap::iterator guildsMapIt = bot->GetGuildsMap().find(guildId);

	if (guildsMapIt == bot->GetGuildsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Bot %s it is not added in guild %s", identifier, guildIdentifier);
		return FALSE;
	}

	const DiscordBot::GuildSlashCommandsMap::const_iterator guildsSlashCommandsIt = bot->GetGuildsSlashCommandsMap().find(guildId);

	if (guildsSlashCommandsIt == bot->GetGuildsSlashCommandsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Could not found guild %s slash commands map for bot %s", guildIdentifier, identifier);
		return FALSE;
	}

	const char* slashCommandIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake slashCommandId(slashCommandIdentifier);
	const dpp::slashcommand_map::const_iterator slashCommandMapIt = guildsSlashCommandsIt->second.find(slashCommandId);

	if (slashCommandMapIt == guildsSlashCommandsIt->second.end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(DeleteGuildSlashCommand) Could not found slash command /%s in slash commands map for guild %s, bot %s", slashCommandIdentifier, guildIdentifier, identifier);
		return FALSE;
	}

	if (slashCommandMapIt->second.id == slashCommandId)
	{
		const std::string slashCommandName = slashCommandMapIt->second.name;
		bot->GetCluster().guild_command_delete(slashCommandId, guildId, [bot, slashCommandIdentifier, slashCommandName, guildId](const dpp::confirmation_callback_t & cb) {
			if (!cb.is_error())
			{
				const bool success = cb.get<dpp::confirmation>().success;
				g_EventsQueue->Push([bot, slashCommandIdentifier, success, slashCommandName, guildId]() {
					if (success)
					{
						DiscordBot::GuildSlashCommandsMap::iterator guildSlashCommandsMapIt = bot->GetGuildsSlashCommandsMap().find(guildId);
						const dpp::snowflake slashCommandId = dpp::snowflake(slashCommandIdentifier);

						if (guildSlashCommandsMapIt == bot->GetGuildsSlashCommandsMap().end())
						{
							gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Slash command %s deleted in Discord API, but not found guild's %s slash commands map", bot->GetIdentifier().c_str(), slashCommandName.c_str(), guildId.str().c_str() );
						}
						else
						{
							dpp::slashcommand_map::iterator slashCommandMapIt = guildSlashCommandsMapIt->second.find(slashCommandId);

							if (slashCommandMapIt == guildSlashCommandsMapIt->second.end())
							{
								gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Slash command %s deleted in Discord API, but not found in guild's %s slash commands map", bot->GetIdentifier().c_str(), slashCommandName.c_str(), guildId.str().c_str());
							}
							else
							{
								guildSlashCommandsMapIt->second.erase(slashCommandId);
							}
						}
					
					}

					ExecuteForward(ON_GUILD_SLASH_COMMAND_DELETE, bot->GetIdentifier().c_str(), success, slashCommandIdentifier, slashCommandName.c_str());
				});
			}
			else
			{
				uint32_t errorCode = cb.get_error().code;
				const std::string errorMessage = cb.get_error().message;
				const std::string humanReadable = cb.get_error().human_readable;

				g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, slashCommandIdentifier, slashCommandName]() {
					ExecuteForward(ON_GUILD_SLASH_COMMAND_DELETE, bot->GetIdentifier().c_str(), false, slashCommandIdentifier, slashCommandName.c_str());

					gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create guild slash command. Code: %i", bot->GetIdentifier().c_str(), errorCode);
					gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
					gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());
				});
			}
		});
	}

	return TRUE;
}
