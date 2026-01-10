#include "precompiled.h"

#include "amx_natives.h"
#include "amx_forwards.h"

void OnAmxxAttach()
{
	MF_PrintSrvConsole("[DiscordAPI] Module loaded successfully!\n");
	MF_PrintSrvConsole("[DiscordAPI] Version %s compiled (%s)\n", MODULE_VERSION, __DATE__);
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

void OnStartFrameDetour()
{
	MF_PrintSrvConsole("SHOULD CRASH IF AMX VM IT'S NOT LOADED");
}