#include <amxxmodule.h>
#include "amxx/amx_forwards.h"

const char* g_ForwardsNames[] = {
	{ "OnBotReady" },
	{ "OnBotStopped" },

	{ "OnGuildCreated"},
	{ "OnGuildDeleted"},

	{ "OnGuildChannelCreate" },
	{ "OnGuildChannelDelete" },

	{ "OnChannelMessageCreated" },

};

ForwardsMap g_Forwards = ForwardsMap();

void RegisterForwards()
{
	g_Forwards[ON_BOT_READY]					= MF_RegisterForward(g_ForwardsNames[ON_BOT_READY], ET_IGNORE, FP_STRING, FP_DONE);
	g_Forwards[ON_BOT_STOPPED]					= MF_RegisterForward(g_ForwardsNames[ON_BOT_STOPPED], ET_IGNORE, FP_STRING, FP_DONE);

	g_Forwards[ON_GUILD_CREATED]				= MF_RegisterForward(g_ForwardsNames[ON_GUILD_CREATED], ET_IGNORE, FP_STRING, FP_STRING, FP_DONE);
	g_Forwards[ON_GUILD_DELETED]				= MF_RegisterForward(g_ForwardsNames[ON_GUILD_DELETED], ET_IGNORE, FP_STRING, FP_STRING, FP_DONE);

	g_Forwards[ON_GUILD_CHANNEL_CREATE]			= MF_RegisterForward(g_ForwardsNames[ON_GUILD_CHANNEL_CREATE], ET_IGNORE, FP_STRING, FP_CELL, FP_CELL, FP_STRING, FP_DONE);
	g_Forwards[ON_GUILD_CHANNEL_DELETE]			= MF_RegisterForward(g_ForwardsNames[ON_GUILD_CHANNEL_DELETE], ET_IGNORE, FP_STRING, FP_CELL, FP_STRING, FP_DONE);

	g_Forwards[ON_CHANNEL_MESSAGE_CREATE]		= MF_RegisterForward(g_ForwardsNames[ON_CHANNEL_MESSAGE_CREATE], ET_IGNORE, FP_STRING, FP_STRING, FP_STRING, FP_DONE);
}