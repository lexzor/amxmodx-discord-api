#include "precompiled.h"

#include "dllmain.h"
#include "amx_natives.h"
#include "amx_forwards.h"
#include "events_queue.h"

void OnAmxxAttach()
{
	RegisterNatives();
}


void OnAmxxDetach()
{

}

void OnPluginsLoaded()
{
	RegisterForwards();

	g_EventsQueue->SetProcessingLock(false);
}

void OnPluginsUnloading()
{
	g_EventsQueue->SetProcessingLock(true);
}

void OnMetaAttach(PLUG_LOADTIME current_phase)
{
	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Success: Version %s compiled (%s)", MODULE_VERSION, MODULE_DATE);
	InitializeEventsQueue();
}

void OnMetaDetach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason)
{
	ConsumeQueueEvents();
	g_EventsQueue.release();
	auto& botMap = DiscordBotsManager::get().GetDiscordBotsMap();
	
	for (auto& bot : botMap)
	{
		bot.second->Stop();
		ExecuteForward(ON_BOT_STOPPED, bot.first.c_str());
		bot.second.release();
	}

	botMap.clear();
	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Module detached");
}