#include "guilds_events_handler.h"

#include "amxxmodule.h"
#include "discord_bot.h"
#include "events_queue.h"
#include "amx_forwards.h"

GuildsEventsHandler::GuildsEventsHandler(DiscordBot* m_Bot)
    : m_Bot(m_Bot)
{
    RegisterListeners();
}

void GuildsEventsHandler::RegisterListeners()
{
    m_Bot->GetCluster().on_guild_create([this](dpp::guild_create_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr || !m_Bot->GetReadyState())
            {
                return;
            }

            OnGuildCreate(cb);
        });
    });

    m_Bot->GetCluster().on_guild_delete([this](dpp::guild_delete_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr || !m_Bot->GetReadyState())
            {
                return;
            }

            OnGuildDelete(cb);
        });
    });

    m_Bot->GetCluster().on_guild_update([this](dpp::guild_update_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr || !m_Bot->GetReadyState())
            {
                return;
            }

            OnGuildUpdate(cb);
        });
    });
}

void GuildsEventsHandler::OnGuildCreate(const dpp::guild_create_t& cb)
{
    m_Bot->GetGuildsMap().emplace(cb.created.id, cb.created);

    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s Bot has been added in '%s' guild\n", m_Bot->GetConsolePrefix().c_str(), cb.created.name.c_str());
    }

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s OnGuildCreate: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.created.to_json().dump(4).c_str());
    }

    dpp::json guild =
    {
        { "id", cb.created.id.str() },
        { "name", cb.created.name }
    };

    ExecuteForward(ON_GUILD_CREATED, m_Bot->GetIdentifier().c_str(), guild.dump().c_str());
}

void GuildsEventsHandler::OnGuildDelete(const dpp::guild_delete_t& cb)
{
    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        if (cb.deleted.is_unavailable())
        {
            MF_PrintSrvConsole("%s '%s' guild has became unavailable (temporarly)\n", m_Bot->GetConsolePrefix().c_str(), cb.deleted.name.c_str());
        }
        else
        {
            MF_PrintSrvConsole("%s Bot was removed from '%s' guild\n", m_Bot->GetConsolePrefix().c_str(), cb.deleted.name.c_str());
        }
    }

    if (!cb.deleted.is_unavailable())
        m_Bot->GetGuildsMap().erase(cb.deleted.id);

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s OnGuildDelete: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.deleted.to_json().dump(4).c_str());
    }

    dpp::json guild =
    {
        { "id", cb.deleted.id.str() },
        { "name", cb.deleted.name },
        { "unavailable", cb.deleted.is_unavailable() }
    };

    ExecuteForward(ON_GUILD_DELETED, m_Bot->GetIdentifier().c_str(), guild.dump().c_str());
}

void GuildsEventsHandler::OnGuildUpdate(const dpp::guild_update_t& cb)
{
    m_Bot->GetGuildsMap()[cb.updated.id] = cb.updated;

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s OnGuildUpdate: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.updated.to_json().dump(4).c_str());
    }

    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s Guild '%s' has been updated\n", m_Bot->GetConsolePrefix().c_str(), cb.updated.name.c_str());
    }

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s OnGuildUpdate: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.updated.to_json().dump(4).c_str());
    }
}