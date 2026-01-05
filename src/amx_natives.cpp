#include "natives/bot.h"
#include "natives/channels.h"

AMX_NATIVE_INFO g_DiscordBotNatives[] =
{
	// bot.h
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
	
	// channels.h
	{ "SendMessageToChannel",				SendMessageToChannel },

	{ NULL, NULL }
};

void RegisterNatives()
{
	MF_AddNatives(g_DiscordBotNatives);
}