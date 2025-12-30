#include <amxxmodule.h>

#include "amx_natives.h"
#include "amx_forwards.h"
#include "discord_bots_manager.h"

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
	MF_PrintSrvConsole("Plugins unloaded\n");
}