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
		{ "GuildChannelExistsById", GuildChannelExistsById },
		{ "GuildChannelExistsByName", GuildChannelExistsByName },
		{ "GetGuildChannel", GetGuildChannel },
		{ "GetGuildChannelsCount", GetGuildChannelsCount },
		{ "GetGuildChannelStringMemberByIndex", GetGuildChannelStringMemberByIndex },
		{ "GetGuildChannelIntegerMemberByIndex", GetGuildChannelIntegerMemberByIndex },
		{ "BeginCreateGuildChannel", BeginCreateGuildChannel },
		{ "EndCreateGuildChannel", EndCreateGuildChannel },
		{ "DeleteGuildChannel", DeleteGuildChannel },
		{ "SetGuildChannelMemberString", SetGuildChannelMemberString },
		{ "SetGuildChannelMemberInteger", SetGuildChannelMemberInteger },
		{ "BeginEditGuildChannel", BeginEditGuildChannel },
		{ "EndEditGuildChannel", EndEditGuildChannel },
		{ "GuildSlashCommandExists", GuildSlashCommandExists },
		{ "BeginCreateGuildSlashCommand", BeginCreateGuildSlashCommand},
		{ "EndCreateGuildSlashCommand", EndCreateGuildSlashCommand },
		{ "AddSlashCommandOption", AddSlashCommandOption },
		{ "BeginCreateSlashCommandOption", BeginCreateSlashCommandOption },
		{ "EndCreateSlashCommandOption", EndCreateSlashCommandOption },
		{ "AddSlashCommandOptionChoiceInteger", AddSlashCommandOptionChoiceInteger },
		{ "AddSlashCommandOptionChoiceString", AddSlashCommandOptionChoiceString },
		{ "AddSlashCommandOptionChoiceFloat", AddSlashCommandOptionChoiceFloat },
		{ "CreateGuildSlashCommand", CreateGuildSlashCommand },
		{ "DeleteGuildSlashCommand", DeleteGuildSlashCommand },

		// channels.h
		{ "SendMessageToChannel", SendMessageToChannel },

		{NULL, NULL}
};

void RegisterNatives()
{
	MF_AddNatives(g_DiscordBotNatives);
}
