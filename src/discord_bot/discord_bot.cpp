#include <amxxmodule.h>
#include "discord_bot/discord_bot.h"
#include "amxx/amx_forwards.h"
#include "dpp/nlohmann/json.hpp"
#include "mpsc/events_queue.h"

DiscordBot::DiscordBot(const std::string &identifier, const std::string &token)
    : m_BotCluster(token, dpp::i_default_intents | dpp::i_message_content, 1), m_Identifier(identifier)
{
    m_LogEventHandler = std::make_unique<LogEventHandler>(this);
    m_ReadyEventHandler = std::make_unique<ReadyEventHandler>(this);
    m_GuildEventsHandler = std::make_unique<GuildsEventsHandler>(this);
    m_MessagesEventsHandler = std::make_unique<MessagesEventsHandler>(this);
}

DiscordBot::~DiscordBot() noexcept {}

void DiscordBot::SetOptions(const DiscordBotOptions &options)
{
    m_Options = options;
}

void DiscordBot::SetEventsDataConsolePrinting(const bool state)
{
    m_Options.print_events_data = state;
}

bool DiscordBot::Start()
{
    if (m_Starting)
    {
        MF_PrintSrvConsole("%s (Start) ERROR: Bot it's under starting state...\n", GetConsolePrefix().c_str());
        return false;
    }

    if (m_Ready)
    {
        MF_PrintSrvConsole("%s (Start) ERROR: Bot it's already marked as ready!\n", GetConsolePrefix().c_str());
        return false;
    }

    
    try {
        m_BotCluster.start(dpp::st_return);
    }
    catch (const dpp::logic_exception& e) {
        MF_PrintSrvConsole("Failed to start bot. Error: %s\n", e.what());
        return false;
    }
    
    m_Starting = true;
    
    return true;
}

bool DiscordBot::Stop()
{
    if(m_Starting)
    {
        MF_PrintSrvConsole("%s (Stop) ERROR: Can't stop bot while starting!\n", GetConsolePrefix().c_str());
        return false;
    }

    if (!m_Ready)
    {
        MF_PrintSrvConsole("%s (Stop) ERROR: Bot it's not ready!\n", GetConsolePrefix().c_str());
        return false;
    }

    try {
        m_BotCluster.shutdown();
    }
    catch (const dpp::logic_exception& e) {
        MF_PrintSrvConsole("Failed to stop bot. Error: %s\n", e.what());
        return false;
    }

    m_Ready = false;
    m_Starting = false;

    ExecuteForward(ON_BOT_STOPPED, m_Identifier.c_str());

    return true;
}

void DiscordBot::SendMessageToChannel(const std::string &channel_id, const std::string &message)
{
    const dpp::snowflake channel(channel_id);

    dpp::message msg(channel, message);
    m_BotCluster.message_create(msg);
}

bool DiscordBot::SendReplyToLastInteraction(const std::string &message)
{
    if (!m_CanSendInteractionMessage)
    {
        MF_PrintSrvConsole("%s There is no active interaction which can receive a reply\n", GetConsolePrefix().c_str());
        return false;
    }

    m_LastInteractionMessage = message;
    return true;
}

void DiscordBot::SetReadyState(bool state)
{
    m_Starting = false;
    m_Ready = state;

    if (GetLogLevel() == LogLevel::VERBOSE)
        MF_PrintSrvConsole("%s Bot has been marked as %sactive\n", GetConsolePrefix().c_str(), m_Ready ? "" : "in");
}

dpp::cluster& DiscordBot::GetCluster() noexcept
{ 
    return m_BotCluster;
}

const std::string& DiscordBot::GetConsolePrefix() const noexcept
{
    return m_Options.chat_prefix;
}

const LogLevel DiscordBot::GetLogLevel() const noexcept
{
    return m_Options.log_level;
}

const bool DiscordBot::IsStarted() const noexcept
{
    return m_Ready || m_Starting;
}

const std::string& DiscordBot::GetIdentifier() const noexcept
{
    return m_Identifier;
}

const DiscordBotOptions& DiscordBot::GetOptions() const noexcept
{
    return m_Options;
}

const std::string& DiscordBot::GetInteractionMessage() const noexcept
{
    return m_LastInteractionMessage;
}

DiscordBot::GuildsMap& DiscordBot::GetGuildsMap() noexcept
{
    return m_Guilds;
}

DiscordBot::SlashCommandsMap& DiscordBot::GetGlobalSlashCommandsMap() noexcept
{
    return m_GlobalSlashCommands;
}

void DiscordBot::SetInteractionReplyAbility(bool state)
{
    m_CanSendInteractionMessage = state;
}

void DiscordBot::SetInteractionMessage(const std::string &message)
{
    m_LastInteractionMessage = message;
}

void DiscordBot::ClearInteractionMessage()
{
    m_LastInteractionMessage.clear();
}

const std::string* DiscordBot::GetLastInteractionMessage() const noexcept
{
    return &m_LastInteractionMessage;
}

const bool DiscordBot::IsDestroyed() const noexcept
{
    return m_IsDestroyed.load();
}