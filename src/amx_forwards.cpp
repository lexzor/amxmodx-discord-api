#include <amxxmodule.h>

#include "amx_forwards.h"

const char* g_ForwardsNames[] = {
	{ "OnBotReady" },
	{ "OnBotStopped" },

	{ "OnGuildCreated"},
	{ "OnGuildDeleted"},

	{ "OnChannelMessageCreated" },
};

ForwardsMap g_Forwards = ForwardsMap();

void RegisterForwards()
{
	g_Forwards[ON_BOT_READY]					= MF_RegisterForward(g_ForwardsNames[ON_BOT_READY], ET_IGNORE, FP_STRING, FP_DONE);
	g_Forwards[ON_BOT_STOPPED]					= MF_RegisterForward(g_ForwardsNames[ON_BOT_STOPPED], ET_IGNORE, FP_STRING, FP_DONE);

	g_Forwards[ON_GUILD_CREATED]				= MF_RegisterForward(g_ForwardsNames[ON_GUILD_CREATED], ET_IGNORE, FP_STRING, FP_STRING, FP_DONE);
	g_Forwards[ON_GUILD_DELETED]				= MF_RegisterForward(g_ForwardsNames[ON_GUILD_DELETED], ET_IGNORE, FP_STRING, FP_STRING, FP_DONE);

	g_Forwards[ON_CHANNEL_MESSAGE_CREATED]		= MF_RegisterForward(g_ForwardsNames[ON_CHANNEL_MESSAGE_CREATED], ET_IGNORE, FP_STRING, FP_STRING, FP_STRING, FP_DONE);
}