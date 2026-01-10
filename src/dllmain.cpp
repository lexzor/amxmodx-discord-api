#include "precompiled.h"

#include "dllmain.h"

#include "amx_natives.h"
#include "amx_forwards.h"

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
}

void OnPluginsUnloaded()
{

}

void OnMetaAttach(PLUG_LOADTIME current_phase)
{
	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Success: Version %s compiled (%s)", MODULE_VERSION, __DATE__);
}

void OnMetaDetach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason)
{
	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Module detached");
}