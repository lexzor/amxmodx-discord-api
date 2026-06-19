#include "guilds.h"
#include "amxx/pending_amx_object_store_impl.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"
#include "utils/strtolower.h"
#include "amx_natives_helpers.h"

cell AMX_NATIVE_CALL GetGuilds(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)

	cell* destination = MF_GetAmxAddr(amx, params[2]);
	int bufferLen = params[3];

	if (bufferLen < 2)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Buffer length must be at least 2", __func__);
		return FALSE;
	}

	memset(destination, 0x0, bufferLen);

	const DiscordBot::GuildsSet& guilds = bot->GetGuildsSet();

	if (guilds.empty())
	{
		destination[0] = static_cast<cell>('[');
		destination[1] = static_cast<cell>(']');
		return FALSE;
	}

	dpp::json jGuilds = dpp::json::array();

	for (const dpp::snowflake& guildId : guilds)
	{
		const dpp::guild* guild = dpp::find_guild(guildId);

		if (guild == nullptr)
			continue;

		jGuilds.push_back({
			{ "id", guild->id.str() },
			{ "name", guild->name }
			});
	}

	const std::string guildsJSON = jGuilds.dump();

	if (guildsJSON.size() > static_cast<size_t>(bufferLen))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Buffer too small, need %zu bytes", __func__, guildsJSON.size());
		return FALSE;
	}

	for (size_t i = 0; i < guildsJSON.size(); i++)
		destination[i] = static_cast<cell>(guildsJSON[i]);

	destination[guildsJSON.size()] = 0x0;

	return static_cast<cell>(guilds.size());
}

cell AMX_NATIVE_CALL GuildChannelExistsById(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* channelIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake requestedChannel(channelIdentifier);

	for (const dpp::snowflake& channel : guild->channels)
	{
		if (channel == requestedChannel)
			return TRUE;
	}

	return FALSE;
}

cell AMX_NATIVE_CALL GuildChannelExistsByName(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* channelName = MF_GetAmxString(amx, params[3], 2, nullptr);

	for (const dpp::snowflake channelId : guild->channels)
	{
		const dpp::channel* channel = dpp::find_channel(channelId);

		if (channel && channel->name == std::string_view(channelName))
			return TRUE;
	}

	return FALSE;
}

cell AMX_NATIVE_CALL GetGuildChannel(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const dpp::guild* guild = dpp::find_guild(guildId);

	if (guild == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s not found in cache", __func__, guildIdentifier);
		return FALSE;
	}

	const char* channelIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::channel* channel = dpp::find_channel(dpp::snowflake(channelIdentifier));

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Channel %s not found in cache", __func__, channelIdentifier);
		return FALSE;
	}

	if (channel->guild_id != guildId)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Channel %s exists but not in guild %s (belongs to guild %s)",
			__func__, channelIdentifier, guildIdentifier, channel->guild_id.str().c_str());
		return FALSE;
	}

	cell* channelNameBuffer = MF_GetAmxAddr(amx, params[4]);
	int channelNameBufferLen = params[5];

	if (channel->name.size() > static_cast<size_t>(channelNameBufferLen))
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Name buffer too small", __func__);
		return FALSE;
	}

	for (size_t i = 0; i < channel->name.size(); i++)
		channelNameBuffer[i] = static_cast<cell>(channel->name[i]);
	channelNameBuffer[channel->name.size()] = 0x0;

	cell* channelParentIdBuffer = MF_GetAmxAddr(amx, params[6]);
	int channelParentIdBufferLen = params[7];

	const std::string parentIdStr = channel->parent_id.str();

	if (parentIdStr.size() > static_cast<size_t>(channelParentIdBufferLen))
	{
		MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Parent ID buffer too small", __func__);
		return FALSE;
	}

	for (size_t i = 0; i < parentIdStr.size(); i++)
		channelParentIdBuffer[i] = static_cast<cell>(parentIdStr[i]);
	channelParentIdBuffer[parentIdStr.size()] = 0x0;

	return TRUE;
}

cell AMX_NATIVE_CALL BeginCreateGuildChannel(AMX* amx, cell* params)
{
	AMX_GET_BOT(-1)
	AMX_GET_GUILD(2, 1, -1)

	PendingAmxObjectStoreHandle handle = g_PendingAmxObjectStore->CreateObject<dpp::channel>();
	dpp::channel* channel = g_PendingAmxObjectStore->GetStoreObject<dpp::channel>(handle);

	channel->set_guild_id(guildId);

	return handle;
}

cell AMX_NATIVE_CALL EndCreateGuildChannel(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_PENDING_OBJECT(dpp::channel, channel, 2, FALSE)

	bot->GetCluster().channel_create(*channel, [bot, channelHandle](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::channel createdChannel = cb.get<dpp::channel>();
			const std::string channelId = createdChannel.id.str();

			dpp::channel* cached = new dpp::channel(createdChannel);
			dpp::get_channel_cache()->store(cached);

			g_EventsQueue->Push([bot, channelId, channelHandle]() {
				ExecuteForward(ON_GUILD_CHANNEL_CREATE, bot->GetIdentifier().c_str(), channelHandle, true, channelId.c_str());
				g_PendingAmxObjectStore->RemoveObject(channelHandle);
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelHandle, humanReadable]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create Discord channel. Code: %u", bot->GetIdentifier().c_str(), errorCode);
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
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const char* channelIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const std::string channelId(channelIdentifier);

	bot->GetCluster().channel_delete(channelId, [bot, channelId, guildId](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::snowflake channelSnowflakeId = dpp::snowflake(channelId);
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
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to delete Discord channel %s. Code: %u", bot->GetIdentifier().c_str(), channelId.c_str(), errorCode);
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
	AMX_GET_PENDING_OBJECT(dpp::channel, channel, 1, FALSE)

	const char* buffer = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(buffer))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Buffer can't be empty", __func__);
		return FALSE;
	}

	enum class ChannelMemberString : uint32_t { NAME, PARENT_ID };

	switch (static_cast<ChannelMemberString>(params[2]))
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
	AMX_GET_PENDING_OBJECT(dpp::channel, channel, 1, FALSE)

	const cell value = params[3];

	enum class ChannelMemberInt : uint32_t { TYPE, FLAGS, USER_LIMIT };

	switch (static_cast<ChannelMemberInt>(params[2]))
	{
	case ChannelMemberInt::TYPE:
		channel->set_type(static_cast<dpp::channel_type>(value));
		break;

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
		if (value < 0 || value > 99)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid USER_LIMIT value %i, must be 0-99", __func__, value);
			return FALSE;
		}
		channel->set_user_limit(value);
		break;
	}

	return TRUE;
}

cell AMX_NATIVE_CALL BeginEditGuildChannel(AMX* amx, cell* params)
{
	AMX_GET_BOT(-1)

	const char* channelIdentifier = MF_GetAmxString(amx, params[2], 1, nullptr);
	dpp::channel* channel = dpp::find_channel(dpp::snowflake(channelIdentifier));

	if (channel == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Channel %s not found in cache", __func__, channelIdentifier);
		return -1;
	}

	return g_PendingAmxObjectStore->CreateObject<dpp::channel>(*channel);
}

cell AMX_NATIVE_CALL EndEditGuildChannel(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_PENDING_OBJECT(dpp::channel, channel, 2, FALSE)

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
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, channelHandle, humanReadable]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to edit Discord channel. Code: %u", bot->GetIdentifier().c_str(), errorCode);
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
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const char* slashCommandName = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(slashCommandName))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Slash command name cannot be empty", __func__);
		return FALSE;
	}

	DiscordBot::GuildSlashCommandsMap& guildsSlashCommandsMap = bot->GetGuildsSlashCommandsMap();
	const DiscordBot::GuildSlashCommandsMap::const_iterator guildsSlashCommandsMapIt = guildsSlashCommandsMap.find(guildId);

	if (guildsSlashCommandsMapIt == guildsSlashCommandsMap.end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s slash commands not yet fetched for bot %s", __func__, guildIdentifier, bot->GetIdentifier().c_str());
		return FALSE;
	}

	cell* slashCommandBuffer = MF_GetAmxAddr(amx, params[4]);
	cell slashCommandBufferLen = params[5];

	for (const auto& [key, value] : guildsSlashCommandsMapIt->second)
	{
		if (value.name != std::string_view(slashCommandName))
			continue;

		if (slashCommandBufferLen == 0)
			return TRUE;

		const std::string slashCommandId = value.id.str();

		if (slashCommandId.size() > static_cast<size_t>(slashCommandBufferLen))
		{
			MF_LogError(amx, AMX_ERR_BOUNDS, "(%s) Slash command ID buffer too small", __func__);
			return FALSE;
		}

		for (size_t i = 0; i < slashCommandId.size(); i++)
			slashCommandBuffer[i] = static_cast<cell>(slashCommandId[i]);
		slashCommandBuffer[slashCommandId.size()] = 0x0;

		return TRUE;
	}

	return FALSE;
}

cell AMX_NATIVE_CALL BeginCreateGuildSlashCommand(AMX* amx, cell* params)
{
	AMX_GET_BOT(-1)

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Slash command must have a name", __func__);
		return -1;
	}

	const char* description = MF_GetAmxString(amx, params[3], 2, nullptr);

	return g_PendingAmxObjectStore->CreateObject<dpp::slashcommand>(strtolower(name), description, bot->GetCluster().me.id);
}

cell AMX_NATIVE_CALL EndCreateGuildSlashCommand(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_PENDING_OBJECT(dpp::slashcommand, slashCommand, 2, FALSE)
	AMX_GET_GUILD(3, 2, FALSE)

	for (auto& opt : slashCommand->options)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "OPT name=%s type=%d choices=%zu",
			opt.name.c_str(), (int)opt.type, opt.choices.size());
	}

	const std::string slashCommandName = slashCommand->name;

	bot->GetCluster().guild_command_create(*slashCommand, guildId, [bot, guildId, slashCommandName, slashCommandHandle](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::slashcommand createdCommand = cb.get<dpp::slashcommand>();
			g_EventsQueue->Push([bot, createdCommand, guildId, slashCommandHandle]() {
				bot->GetGuildsSlashCommandsMap()[guildId][createdCommand.id] = createdCommand;
				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), true, createdCommand.name.c_str(), createdCommand.id.str().c_str());
				g_PendingAmxObjectStore->RemoveObject(slashCommandHandle);
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, slashCommandName, slashCommandHandle]() {
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create guild slash command %s. Code: %u", bot->GetIdentifier().c_str(), slashCommandName.c_str(), errorCode);
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
	AMX_GET_PENDING_OBJECT(dpp::slashcommand, slashCommand, 1, FALSE)

	const cell slashCommandOptionType = params[2];

	if (slashCommandOptionType < 1 || slashCommandOptionType > 11)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid slash command option type %i", __func__, slashCommandOptionType);
		return FALSE;
	}

	const char* commandName = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(commandName))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Slash command option must have a name", __func__);
		return FALSE;
	}

	const char* commandDescription = MF_GetAmxString(amx, params[4], 3, nullptr);

	slashCommand->add_option(dpp::command_option(
		static_cast<dpp::command_option_type>(slashCommandOptionType),
		strtolower(commandName),
		commandDescription,
		static_cast<bool>(params[5])
	));

	return TRUE;
}

cell AMX_NATIVE_CALL BeginCreateSlashCommandOption(AMX* amx, cell* params)
{
	const cell slashCommandOptionType = params[1];

	if (slashCommandOptionType < 1 || slashCommandOptionType > 11)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid slash command option type %i", __func__, slashCommandOptionType);
		return -1;
	}

	const char* commandName = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(commandName))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Slash command option must have a name", __func__);
		return -1;
	}

	const char* commandDescription = MF_GetAmxString(amx, params[3], 2, nullptr);

	return g_PendingAmxObjectStore->CreateObject<dpp::command_option>(
		static_cast<dpp::command_option_type>(slashCommandOptionType),
		strtolower(commandName),
		commandDescription,
		static_cast<bool>(params[4])
	);
}

cell AMX_NATIVE_CALL EndCreateSlashCommandOption(AMX* amx, cell* params)
{
	AMX_GET_PENDING_OBJECT(dpp::slashcommand, slashCommand, 2, FALSE)

	const cell slashCommandOptionHandle = params[1];
	dpp::command_option* slashCommandOption = g_PendingAmxObjectStore->GetStoreObject<dpp::command_option>(slashCommandOptionHandle);

	if (slashCommandOption == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid command_option handle %i", __func__, slashCommandOptionHandle);
		return FALSE;
	}

	slashCommand->add_option(*slashCommandOption);
	g_PendingAmxObjectStore->RemoveObject(slashCommandOptionHandle);

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceInteger(AMX* amx, cell* params)
{
	AMX_GET_PENDING_OBJECT(dpp::command_option, slashCommandOption, 1, FALSE)

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Choice must have a name", __func__);
		return FALSE;
	}

	slashCommandOption->add_choice(dpp::command_option_choice(strtolower(name), static_cast<int64_t>(params[3])));

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceString(AMX* amx, cell* params)
{
	AMX_GET_PENDING_OBJECT(dpp::command_option, slashCommandOption, 1, FALSE)

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Choice must have a name", __func__);
		return FALSE;
	}

	const char* value = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(value))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Choice must have a value", __func__);
		return FALSE;
	}

	slashCommandOption->add_choice(dpp::command_option_choice(strtolower(name), std::string(value)));

	return TRUE;
}

cell AMX_NATIVE_CALL AddSlashCommandOptionChoiceFloat(AMX* amx, cell* params)
{
	AMX_GET_PENDING_OBJECT(dpp::command_option, slashCommandOption, 1, FALSE)

	const char* name = MF_GetAmxString(amx, params[2], 1, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Choice must have a name", __func__);
		return FALSE;
	}

	slashCommandOption->add_choice(dpp::command_option_choice(strtolower(name), static_cast<double>(amx_ctof(params[3]))));

	return TRUE;
}

cell AMX_NATIVE_CALL CreateGuildSlashCommand(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const DiscordBot::GuildSlashCommandsMap::const_iterator guildsSlashCommandsIt = bot->GetGuildsSlashCommandsMap().find(guildId);

	if (guildsSlashCommandsIt == bot->GetGuildsSlashCommandsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s slash commands map not found for bot %s", __func__, guildIdentifier, identifier);
		return FALSE;
	}

	const char* name = MF_GetAmxString(amx, params[3], 2, nullptr);

	if (!strlen(name))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Slash command must have a name", __func__);
		return FALSE;
	}

	const char* description = MF_GetAmxString(amx, params[4], 3, nullptr);
	const dpp::slashcommand slashCommand(strtolower(name), description, bot->GetCluster().me.id);
	const std::string slashCommandName = slashCommand.name;

	bot->GetCluster().guild_command_create(slashCommand, guildId, [bot, guildId, slashCommandName](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const dpp::slashcommand createdCommand = cb.get<dpp::slashcommand>();
			g_EventsQueue->Push([bot, createdCommand, guildId]() {
				bot->GetGuildsSlashCommandsMap()[guildId][createdCommand.id] = createdCommand;
				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), true, createdCommand.name.c_str(), createdCommand.id.str().c_str());
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, slashCommandName]() {
				ExecuteForward(ON_GUILD_SLASH_COMMAND_CREATE, bot->GetIdentifier().c_str(), false, slashCommandName.c_str(), "");
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to create guild slash command %s. Code: %u", bot->GetIdentifier().c_str(), slashCommandName.c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());
			});
		}
	});

	return TRUE;
}

cell AMX_NATIVE_CALL DeleteGuildSlashCommand(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)
	AMX_GET_GUILD(2, 1, FALSE)

	const DiscordBot::GuildSlashCommandsMap::const_iterator guildsSlashCommandsIt = bot->GetGuildsSlashCommandsMap().find(guildId);

	if (guildsSlashCommandsIt == bot->GetGuildsSlashCommandsMap().end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Guild %s slash commands map not found for bot %s", __func__, guildIdentifier, identifier);
		return FALSE;
	}

	const char* slashCommandIdentifier = MF_GetAmxString(amx, params[3], 2, nullptr);
	const dpp::snowflake slashCommandId(slashCommandIdentifier);
	const dpp::slashcommand_map::const_iterator slashCommandMapIt = guildsSlashCommandsIt->second.find(slashCommandId);

	if (slashCommandMapIt == guildsSlashCommandsIt->second.end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Slash command %s not found in guild %s for bot %s", __func__, slashCommandIdentifier, guildIdentifier, identifier);
		return FALSE;
	}

	const std::string slashCommandName = slashCommandMapIt->second.name;

	bot->GetCluster().guild_command_delete(slashCommandId, guildId, [bot, slashCommandIdentifier, slashCommandName, guildId](const dpp::confirmation_callback_t& cb) {
		if (!cb.is_error())
		{
			const bool success = cb.get<dpp::confirmation>().success;
			g_EventsQueue->Push([bot, slashCommandIdentifier, success, slashCommandName, guildId]() {
				if (success)
				{
					DiscordBot::GuildSlashCommandsMap::iterator guildSlashCommandsMapIt = bot->GetGuildsSlashCommandsMap().find(guildId);

					if (guildSlashCommandsMapIt == bot->GetGuildsSlashCommandsMap().end())
					{
						gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Slash command %s deleted in API but guild %s not found in map",
							bot->GetIdentifier().c_str(), slashCommandName.c_str(), guildId.str().c_str());
					}
					else
					{
						const dpp::snowflake slashCommandId(slashCommandIdentifier);
						dpp::slashcommand_map::iterator it = guildSlashCommandsMapIt->second.find(slashCommandId);

						if (it == guildSlashCommandsMapIt->second.end())
							gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Slash command %s deleted in API but not found in guild %s map",
								bot->GetIdentifier().c_str(), slashCommandName.c_str(), guildId.str().c_str());
						else
							guildSlashCommandsMapIt->second.erase(it);
					}
				}

				ExecuteForward(ON_GUILD_SLASH_COMMAND_DELETE, bot->GetIdentifier().c_str(), success, slashCommandIdentifier, slashCommandName.c_str());
			});
		}
		else
		{
			const uint32_t errorCode = cb.get_error().code;
			const std::string errorMessage = cb.get_error().message;
			const std::string humanReadable = cb.get_error().human_readable;

			g_EventsQueue->Push([bot, errorCode, errorMessage, humanReadable, slashCommandIdentifier, slashCommandName]() {
				ExecuteForward(ON_GUILD_SLASH_COMMAND_DELETE, bot->GetIdentifier().c_str(), false, slashCommandIdentifier, slashCommandName.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Failed to delete guild slash command. Code: %u", bot->GetIdentifier().c_str(), errorCode);
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Message: %s", bot->GetIdentifier().c_str(), errorMessage.c_str());
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] (%s) Human readable error: %s", bot->GetIdentifier().c_str(), humanReadable.c_str());
			});
		}
	});

	return TRUE;
}