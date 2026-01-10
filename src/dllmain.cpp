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
	MF_PrintSrvConsole("[DiscordAPI] Module attached successfully!\n");
	MF_PrintSrvConsole("[DiscordAPI] Version %s compiled (%s)\n", MODULE_VERSION, __DATE__);
}

void OnMetaDetach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason)
{
	MF_PrintSrvConsole("[DiscordAPI] Module detached\n");
}