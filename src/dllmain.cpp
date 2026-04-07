#include "precompiled.h"

#include "dllmain.h"
#include "amx_natives.h"
#include "amx_forwards.h"
#include "events_queue.h"
#include "console_commands/concmds.h"

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
	InitializeEventsQueue();
	RegisterConsoleCommands();
	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Success: Version %s compiled (%s)", MODULE_VERSION, MODULE_DATE);
}

void OnMetaDetach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason)
{
	ConsumeQueueEvents();
	g_EventsQueue.reset();
	g_DiscordBotsManager.reset();
	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Module detached");
}