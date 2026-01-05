#include <amxxmodule.h>

#include "amx_natives.h"
#include "amx_forwards.h"
#include "discord_bots_manager.h"
#include "moduleconfig.h"

void OnAmxxAttach()
{
	MF_PrintSrvConsole("[DiscordAPI] Module loaded successfully!\n");
	MF_PrintSrvConsole("[DiscordAPI] Version %s compiled %s\n", MODULE_VERSION, __DATE__);
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