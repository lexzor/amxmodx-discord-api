#include "guilds_events_handler.h"

#include "amxxmodule.h"
#include "discord_bot/discord_bot.h"
#include "mpsc/events_queue.h"
#include "amxx/amx_forwards.h"

GuildsEventsHandler::GuildsEventsHandler(DiscordBot* bot)
    : m_Bot(bot)
{
    RegisterListeners();
}

GuildsEventsHandler::~GuildsEventsHandler()
{
    m_Bot = nullptr;
}

void GuildsEventsHandler::RegisterListeners()
{
    m_Bot->GetCluster().on_guild_create([this](dpp::guild_create_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;

            OnGuildCreate(cb);
        });
    });

    m_Bot->GetCluster().on_guild_delete([this](dpp::guild_delete_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;

            OnGuildDelete(cb);
        });
    });

    m_Bot->GetCluster().on_guild_update([this](dpp::guild_update_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;

            OnGuildUpdate(cb);
        });
    });
}

void GuildsEventsHandler::OnGuildCreate(const dpp::guild_create_t& cb)
{
    m_Bot->GetGuildsSet().insert(cb.created.id);

    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
        MF_PrintSrvConsole("%s Bot has been added in '%s' guild\n", m_Bot->GetConsolePrefix().c_str(), cb.created.name.c_str());

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
        MF_PrintSrvConsole("%s OnGuildCreate: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.created.to_json().dump(4).c_str());

    const std::string guildId = cb.created.id.str();
    const std::string guildName = cb.created.name;
    
    m_Bot->GetCluster().guild_commands_get(cb.created.id, [this, guildId, guildName](const dpp::confirmation_callback_t& cb) {
        if (m_Bot == nullptr)
            return;

        if (cb.is_error())
        {
            const std::string errorMessage = cb.get_error().human_readable;

            g_EventsQueue->Push([this, errorMessage, guildId, guildName]() {
                if (m_Bot == nullptr)
                    return;

                MF_PrintSrvConsole("%s ERROR: Failed to retrieve guild %s (%s) slash commands from Discord API\n", m_Bot->GetConsolePrefix().c_str(), guildId.c_str(), guildName.c_str());
                MF_PrintSrvConsole("%s Message: %s\n", m_Bot->GetConsolePrefix().c_str(), errorMessage.c_str());
            });
        }
        else
        {
            dpp::slashcommand_map cmdsMap;
            cmdsMap = std::get<dpp::slashcommand_map>(cb.value);

            g_EventsQueue->Push([this, cmdsMap, guildId, guildName]() {
                if (m_Bot == nullptr)
                    return;

                std::size_t slashCommandsCount = cmdsMap.size();

                DiscordBot::GuildSlashCommandsMap& botCmdsMap = m_Bot->GetGuildsSlashCommandsMap();
                botCmdsMap[dpp::snowflake(guildId)] = {};

                if (slashCommandsCount > 0)
                {
                    botCmdsMap[dpp::snowflake(guildId)] = cmdsMap;

                    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
                        MF_PrintSrvConsole("%s Retrieved %i guild slash command%s from Discord API for %s (%s)\n", m_Bot->GetConsolePrefix().c_str(), slashCommandsCount, slashCommandsCount == 1 ? "s" : "", guildId.c_str(), guildName.c_str());
                }
                else if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
                    MF_PrintSrvConsole("%s No guild slash commands are registered for this bot on Discord API for %s (%s)\n", m_Bot->GetConsolePrefix().c_str(), guildId.c_str(), guildName.c_str());
                }
            );
        }
        
        ExecuteForward(ON_GUILD_CREATED, m_Bot->GetIdentifier().c_str(), guildId.c_str(), guildName.c_str());
    });
}

void GuildsEventsHandler::OnGuildDelete(const dpp::guild_delete_t& cb)
{
    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
        if (cb.deleted.is_unavailable())
            MF_PrintSrvConsole("%s '%s' guild has became unavailable (temporarly)\n", m_Bot->GetConsolePrefix().c_str(), cb.deleted.name.c_str());
        else
            MF_PrintSrvConsole("%s Bot was removed from '%s' guild\n", m_Bot->GetConsolePrefix().c_str(), cb.deleted.name.c_str());

    if (!cb.deleted.is_unavailable())
        m_Bot->GetGuildsSet().erase(cb.deleted.id);

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
        MF_PrintSrvConsole("%s OnGuildDelete: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.deleted.to_json().dump(4).c_str());

    ExecuteForward(ON_GUILD_DELETED, m_Bot->GetIdentifier().c_str(), cb.deleted.id.str().c_str(), cb.deleted.name.c_str(), cb.deleted.is_unavailable());
}

void GuildsEventsHandler::OnGuildUpdate(const dpp::guild_update_t& cb)
{
    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
        MF_PrintSrvConsole("%s OnGuildUpdate: \n%s\n", m_Bot->GetConsolePrefix().c_str(), cb.updated.to_json().dump(4).c_str());

    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
        MF_PrintSrvConsole("%s Guild '%s' has been updated\n", m_Bot->GetConsolePrefix().c_str(), cb.updated.name.c_str());
}