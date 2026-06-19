#include "channels.h"
#include "amx_natives_helpers.h"

cell AMX_NATIVE_CALL SendMessageToChannel(AMX* amx, cell* params)
{
	AMX_GET_BOT(FALSE)

	const char* channelId = MF_GetAmxString(amx, params[2], 1, nullptr);
	const char* message = MF_GetAmxString(amx, params[3], 2, nullptr);

	bot->SendMessageToChannel(channelId, message);
	return TRUE;
}