#pragma once

#include <concepts>
#include "discord_bot/discord_bots_manager_impl.h"

#pragma pack(push)
#include "extdll.h"
#include "meta_api.h"
#include "dllapi.h"
#include "enginecallbacks.h"
#include "usercmd.h"
#include "amxxmodule.h"
#pragma pack(pop)

#define MAX_IDENTIFIER_LENGTH           32
#define MAX_CONSOLE_PREFIX_LENGTH       32