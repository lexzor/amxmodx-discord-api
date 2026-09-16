#pragma once

#define AMX_GET_BOT(ret_val)                                                                          \
    const char* identifier = MF_GetAmxString(amx, params[1], 0, nullptr);                             \
    DiscordBot* bot = g_DiscordBotsManager->GetBotRawPtrByIdentifier(identifier);                     \
    if (bot == nullptr)                                                                               \
    {                                                                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Bot with identifier '%s' does not exist",              \
            __func__, identifier);                                                                    \
        return ret_val;                                                                               \
    }                                                                                                 \
    if (!bot->IsStarted())                                                                            \
    {                                                                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Bot with identifier '%s' is not ready",                \
            __func__, identifier);                                                                    \
        return ret_val;                                                                               \
    }

#define AMX_GET_GUILD(param_index, str_index, ret_val)                                                \
    const char* guildIdentifier = MF_GetAmxString(amx, params[param_index], str_index, nullptr);      \
    const dpp::snowflake guildId = dpp::snowflake(guildIdentifier);                                   \
    if (!bot->GetGuildsSet().count(guildId))                                                          \
    {                                                                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Bot '%s' is not in guild '%s'",                        \
            __func__, identifier, guildIdentifier);                                                   \
        return ret_val;                                                                               \
    }

#define AMX_GET_PENDING_OBJECT(type, var_name, param_index, ret_val)                                  \
    cell var_name##Handle = params[param_index];                                                      \
    type* var_name = g_PendingAmxObjectStore->GetStoreObject<type>(var_name##Handle);                 \
    if (var_name == nullptr)                                                                          \
    {                                                                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Invalid " #type " handle %i",                          \
            __func__, var_name##Handle);                                                              \
        return ret_val;                                                                               \
    }

#define AMX_GET_MEMBER(param_index, str_index, ret_val)                                               \
    const char* memberIdentifier = MF_GetAmxString(amx, params[param_index], str_index, nullptr);     \
    const dpp::snowflake memberId = dpp::snowflake(memberIdentifier);                                 \
    const dpp::guild_member* member = nullptr;                                                        \
    {                                                                                                 \
        auto memberIt = guild->members.find(memberId);                                                \
        if (memberIt != guild->members.end())                                                         \
        {                                                                                             \
            member = &memberIt->second;                                                               \
        }                                                                                             \
    }                                                                                                 \
    if (member == nullptr)                                                                            \
    {                                                                                                 \
        MF_LogError(amx, AMX_ERR_NATIVE, "(%s) Member '%s' not found in guild '%s'",                  \
            __func__, memberIdentifier, guildIdentifier);                                             \
        return ret_val;                                                                               \
    }