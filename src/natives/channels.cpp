#include "channels.h"
#include "common.h"

cell AMX_NATIVE_CALL SendMessageToChannel(AMX* amx, cell* params)
{
	const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);

	DiscordBot* bot = DiscordBotsManager::get().GetBotRawPtrByIdentifier(identifier);

	if (bot == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SendMessageToChannel) Bot with identifier '%s' does not exists", identifier);
		return FALSE;
	}

	if (!bot->GetReadyState())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "(SendMessageToChannel) Bot with identifier '%s' is ready", identifier);
		return FALSE;
	}

	const char* channelId = MF_GetAmxString(amx, params[2], 1, nullptr);
	const char* message = MF_GetAmxString(amx, params[3], 2, nullptr);

	bot->SendMessageToChannel(channelId, message);

	return TRUE;
}