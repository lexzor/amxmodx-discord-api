#include "precompiled.h"

#include "dllmain.h"
#include "amxx/amx_natives.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"
#include "console_commands/concmds.h"
#include "console_variables/cvars.h"

void OnAmxxAttach()
{
	RegisterNatives();
}

void OnAmxxDetach() {}

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
	InitializeDiscordBotsManager();
	InitializeEventsQueue();
	RegisterConsoleCommands();
	RegisterConsoleVariables();

	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Success: Version %s compiled (%s)", MODULE_VERSION, MODULE_DATE);
}

void OnMetaDetach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason)
{
	ConsumeQueueEvents();
	DeinitializeDiscordBotsManager();

	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Module detached");
}