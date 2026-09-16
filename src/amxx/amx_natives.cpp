#include "natives/module.h"
#include "natives/bot.h"
#include "natives/guilds.h"
#include "natives/members.h"
#include "natives/channels.h"
#include "natives/roles.h"

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
		{ "GetGuildChannelStringMemberById", GetGuildChannelStringMemberById },
		{ "GetGuildChannelIntegerMemberById", GetGuildChannelIntegerMemberById },
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

		// members.h
		{ "GuildMemberExistsById", GuildMemberExistsById },
		{ "GuildMemberExistsByUsername", GuildMemberExistsByUsername },
		{ "GetGuildMemberIdByUsername", GetGuildMemberIdByUsername },
		{ "GetGuildMemberStringMemberById", GetGuildMemberStringMemberById },
		{ "GetGuildMemberIntegerMemberById", GetGuildMemberIntegerMemberById },
		{ "GetGuildMembersCount", GetGuildMembersCount },
		{ "GetGuildMemberStringMemberByIndex", GetGuildMemberStringMemberByIndex },
		{ "GetGuildMemberIntegerMemberByIndex", GetGuildMemberIntegerMemberByIndex },
		{ "GuildMemberHasRole", GuildMemberHasRole },
		{ "AddGuildMemberRole", AddGuildMemberRole },
		{ "RemoveGuildMemberRole", RemoveGuildMemberRole },
		{ "FetchGuildUserById", FetchGuildUserById },
		{ "FetchGuildUserByUsername", FetchGuildUserByUsername },

		// roles.h
		{ "GuildRoleExistsById", GuildRoleExistsById },
		{ "GuildRoleExistsByName", GuildRoleExistsByName },
		{ "GetGuildRoleIdByName", GetGuildRoleIdByName },

		// channels.h
		{ "SendMessageToChannel", SendMessageToChannel },

		{NULL, NULL}
};

void RegisterNatives()
{
	MF_AddNatives(g_DiscordBotNatives);
}
