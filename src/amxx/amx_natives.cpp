#include "natives/module.h"
#include "natives/bot.h"
#include "natives/guilds.h"
#include "natives/channels.h"

AMX_NATIVE_INFO g_DiscordBotNatives[] =
	{	
		// module.h
		{ "GetModuleVersionString", GetModuleVersionString },

		// bot.h
		{ "CreateBot", CreateBot },
		{ "SetBotOptions", SetBotOptions },
		{ "BotExists", BotExists },
		{ "IsBotReady", IsBotReady },
		{ "StartBot", StartBot },
		{ "StopBot", StopBot },
		{ "DeleteBot", DeleteBot },
		{ "SendReply", SendReply },

		// guilds.h
		{ "GetGuilds", GetGuilds },
		{ "GuildChannelExists", GuildChannelExists },

		// channels.h
		{ "SendMessageToChannel", SendMessageToChannel },

		{NULL, NULL}};

void RegisterNatives()
{
	MF_AddNatives(g_DiscordBotNatives);
}