#include "bot.h"

#include <algorithm>
#include "parse_discord_bot_options.h"

cell AMX_NATIVE_CALL CreateBot(AMX *amx, cell *params)
{
	int identifierLen = 0;
	int tokenLen = 0;

	const char *identifier = MF_GetAmxString(amx, params[1], 0, &identifierLen);
	const char *token = MF_GetAmxString(amx, params[2], 1, &tokenLen);

	if (identifierLen == 0 || identifierLen >= MAX_IDENTIFIER_LENGTH)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(CreateBot) Discord bot identifier string can't be empty or longer than %i characters!", MAX_IDENTIFIER_LENGTH);
		return FALSE;
	}

	if (tokenLen == 0)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(CreateBot) Discord bot token string can't be empty!");
		return FALSE;
	}

	return static_cast<cell>(g_DiscordBotsManager->InitializeBot(identifier, token));
}

cell AMX_NATIVE_CALL SetBotOptions(AMX *amx, cell *params)
{
	const char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot *bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SetBotOptions) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	const cell *options = MF_GetAmxAddr(amx, params[2]);

	bot->SetOptions(ParseBotOptionsFromAmxArray(options));

	return TRUE;
}

cell AMX_NATIVE_CALL StartBot(AMX *amx, cell *params)
{
	const char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot *bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(StartBot) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (bot->GetReadyState())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(StartBot) Bot with identifier '%s' is ready", identifier);
		return FALSE;
	}

	return static_cast<cell>(bot->Start());
}

cell AMX_NATIVE_CALL BotExists(AMX *amx, cell *params)
{
	char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	return g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier) != nullptr ? TRUE : FALSE;
}

cell AMX_NATIVE_CALL IsBotReady(AMX *amx, cell *params)
{
	const char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot *bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(UnregisterSlashCommand) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	return static_cast<cell>(bot->GetReadyState());
}

cell AMX_NATIVE_CALL StopBot(AMX *amx, cell *params)
{
	const char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot *bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(StopBot) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->GetReadyState())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(StopBot) Bot with identifier '%s' is not ready yet", identifier);
		return FALSE;
	}

	return static_cast<cell>(bot->Stop());
}

cell AMX_NATIVE_CALL DeleteBot(AMX *amx, cell *params)
{
	char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	return static_cast<cell>(g_DiscordBotsManager->DeinitializeBot(identifier));
}

cell AMX_NATIVE_CALL SendReply(AMX *amx, cell *params)
{
	const char *identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot *bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SendReply) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->GetReadyState())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SendReply) Bot with identifier '%s' is ready", identifier);
		return FALSE;
	}

	const char *replyMessage = MF_GetAmxString(amx, params[2], 1, nullptr);

	return static_cast<cell>(bot->SendReplyToLastInteraction(replyMessage));
}