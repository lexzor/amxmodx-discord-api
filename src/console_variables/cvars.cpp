#include "cvars.h"

#include "precompiled.h"
#include "moduleconfig.h"

cvar_t g_cvDiscordAPIVersion =
{
    .name  = const_cast<char*>("discordapi_version"),
    .string = const_cast<char*>(MODULE_VERSION),
    .flags = FCVAR_SERVER | FCVAR_EXTDLL | FCVAR_UNLOGGED | FCVAR_SPONLY,
    .value = 0.0f,
    .next  = nullptr
};

void RegisterConsoleVariables()
{
    CVAR_REGISTER(const_cast<cvar_t*>(&g_cvDiscordAPIVersion));
}