#include <algorithm>
#include "amx_natives.h"
#include "common.h"
#include "utils/parse_discord_bot_options.h"

cell AMX_NATIVE_CALL CreateBot(AMX* amx, cell* params)
{
	int identifierLen = 0;
	int tokenLen = 0;

	const char* identifier = MF_GetAmxString(amx, params[1], 0, &identifierLen);
	const char* token = MF_GetAmxString(amx, params[2], 1, &tokenLen);

	if (identifierLen == 0)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(CreateBot) Discord bot identifier string can't be empty!");
		return FALSE;
	}

	if (tokenLen == 0)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(CreateBot) Discord bot token string can't be empty!");
		return FALSE;
	}

	return static_cast<cell>(DiscordBotsManager::get().InitializeBot(identifier, token));
}

cell AMX_NATIVE_CALL SetBotOptions(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SetBotOptions) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	const cell* options = MF_GetAmxAddr(amx, params[2]);

	bot->SetOptions(ParseBotOptionsFromAmxArray(options));

	return TRUE;
}

cell AMX_NATIVE_CALL BotExists(AMX* amx, cell* params)
{
	char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	return DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier) != nullptr ? TRUE : FALSE;
}

cell AMX_NATIVE_CALL StartBot(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot =  DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);
	
	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(StartBot) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}
	
	return static_cast<cell>(bot->Start());
}

cell AMX_NATIVE_CALL StopBot(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(StopBot) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	return static_cast<cell>(bot->Stop());
}

cell AMX_NATIVE_CALL RegisterGlobalSlashCommand(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(RegisterGlobalSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	const char* command = MF_GetAmxString(amx, params[2], 1, nullptr);
	const char* description = MF_GetAmxString(amx, params[3], 2, nullptr);
	const char* callback = MF_GetAmxString(amx, params[4], 3, nullptr);

	const int fwid = MF_RegisterSPForwardByName(amx, callback, FP_STRING, FP_DONE);

	if (fwid == -1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(RegisterGlobalSlashCommand) Failed to register AMXX forward '%s' of slash command '%s' for bot '%s'", callback, command, identifier);
		return FALSE;
	}

	bot->RegisterGlobalSlashCommand(command, description, fwid);

	return TRUE;
}

cell AMX_NATIVE_CALL DeleteBot(AMX* amx, cell* params)
{
	char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	return static_cast<cell>(DiscordBotsManager::get().DeinitializeBot(identifier));
}

cell AMX_NATIVE_CALL UnregisterGlobalSlashCommand(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(UnregisterGlobalSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	std::string command(MF_GetAmxString(amx, params[2], 1, nullptr));

	DiscordBot::SlashCommandsMap globalCmdsMap = bot->GetGlobalSlashCommandsMap();

	auto it = std::find_if(globalCmdsMap.begin(), globalCmdsMap.end(),
		[&](const std::pair<const dpp::snowflake, SlashCommand>& pair) {
			return pair.second.slash_command.name == command;
		});

	if (it == globalCmdsMap.end())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(UnregisterGlobalSlashCommand) Slash command '%s' does not exists!", command.c_str());
		return FALSE;
	}

	bot->UnregisterGlobalSlashCommand(it->first, command);

	return TRUE;
}

cell AMX_NATIVE_CALL ClearGlobalSlashCommands(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(ClearGlobalSlashCommands) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	DiscordBot::SlashCommandsMap globalCmdsMap = bot->GetGlobalSlashCommandsMap();

	if (globalCmdsMap.size() == 0)
	{
		MF_PrintSrvConsole("%s (ClearGlobalSlashCommands) Bot '%s' does not have registered slash commands\n", bot->GetConsolePrefix().c_str());
		return FALSE;
	}

	MF_PrintSrvConsole("%s (ClearGlobalSlashCommands) Clearing slash commands from bot...\n", bot->GetConsolePrefix().c_str());

	for (const auto& [snowflake, slashcmd] : globalCmdsMap)
	{
		bot->UnregisterGlobalSlashCommand(snowflake, slashcmd.slash_command.name);
	}

	return TRUE;
}

cell AMX_NATIVE_CALL IsBotReady(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(UnregisterSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	return static_cast<cell>(bot->GetReadyState());
}

cell AMX_NATIVE_CALL SendReply(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);
	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SendReply) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	const char* replyMessage = MF_GetAmxString(amx, params[2], 1, nullptr);

	return static_cast<cell>(bot->SendReplyToLastInteraction(replyMessage));
}

cell AMX_NATIVE_CALL SendMessageToChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SendMessageToChannel) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	const char* channelId = MF_GetAmxString(amx, params[2], 1, nullptr);
	const char* message = MF_GetAmxString(amx, params[3], 2, nullptr);

	bot->SendMessageToChannel(channelId, message);

	return TRUE;
}

AMX_NATIVE_INFO g_DiscordBotNatives[] =
{
	{ "CreateBot",							CreateBot },
	{ "SetBotOptions",						SetBotOptions },
	{ "BotExists",							BotExists },
	{ "IsBotReady", 						IsBotReady },
	{ "StartBot",							StartBot },
	{ "StopBot",							StopBot },
	{ "DeleteBot",							DeleteBot },
	{ "RegisterGlobalSlashCommand",			RegisterGlobalSlashCommand },
	{ "UnregisterGlobalSlashCommand", 		UnregisterGlobalSlashCommand },
	{ "ClearGlobalSlashCommands",			ClearGlobalSlashCommands },
	{ "SendReply",							SendReply },
	{ "SendMessageToChannel",				SendMessageToChannel },

	{ NULL, NULL }
};

void RegisterNatives()
{
	MF_AddNatives(g_DiscordBotNatives);
}