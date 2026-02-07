#include <amxxmodule.h>
#include "discord_bot.h"
#include "amx_forwards.h"
#include "dpp/nlohmann/json.hpp"
#include "events_queue.h"

DiscordBot::DiscordBot(const std::string& identifier, const std::string& token)
    : m_BotCluster(token, dpp::i_default_intents | dpp::i_message_content, 1), m_Options(DiscordBotOptions()), m_Identifier(identifier)
{
	m_GuildEventsHandler = std::make_unique<GuildsEventsHandler>(this);
}

DiscordBot::~DiscordBot()
{
    m_BotCluster.shutdown();
    m_GuildEventsHandler.reset();
}

void DiscordBot::SetOptions(const DiscordBotOptions& options)
{
    m_Options = options;
}

void DiscordBot::SetEventsDataConsolePrinting(const bool state)
{
	m_Options.print_events_data = state;
}

bool DiscordBot::Start()
{
    if (m_Ready)
    {
        MF_PrintSrvConsole("%s (Start) ERROR: Bot it's already marked as ready!\n", GetConsolePrefix().c_str());
        return false;
    }

    m_BotCluster.start(dpp::st_return);
    return true;
}

bool DiscordBot::Stop()
{
    if (!m_Ready)
    {
        MF_PrintSrvConsole("%s (Stop) ERROR: Bot it's not ready!\n", GetConsolePrefix().c_str());
        return false;
    }

    m_BotCluster.shutdown();
	ExecuteForward(ON_BOT_STOPPED, m_Identifier.c_str());
    return true;
}

void DiscordBot::RegisterGlobalSlashCommand(const std::string& command, const std::string& description, int amx_fw_handle)
{
    for (auto& [_, value] : m_GlobalSlashCommands)
    {
        if (value.slash_command.name == command)
        {
            if (value.amx_fw_handle == INVALID_SLASH_COMMAND_AMXX_FW_HANDLE)
            {
                value.amx_fw_handle = amx_fw_handle;
                return;
            }

            MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) Slash command '%s' already registered.\n", command.c_str());
            return;
        }
    }

    m_BotCluster.global_command_create(
        { command, description, m_BotCluster.me.id },
        [this, command, amx_fw_handle](const dpp::confirmation_callback_t& cb) {
            if (cb.is_error())
            {
                MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) ERROR: Failed to register slash command '%s'\n", GetConsolePrefix().c_str(), command.c_str());
                MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) %s\n", GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
            }
            else
            {
                const dpp::slashcommand& createdSlashCmd = std::get<dpp::slashcommand>(cb.value);
                const dpp::snowflake snowflake = createdSlashCmd.id;

                m_GlobalSlashCommands.emplace(snowflake, SlashCommand(createdSlashCmd, amx_fw_handle));

                if (GetLogLevel() == LogLevel::DEFAULT || GetLogLevel() == LogLevel::VERBOSE)
                {
                    MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) Slash command '%s' has been registered succesfully\n", GetConsolePrefix().c_str(), command.c_str());
                }
            }
        }
    );

    return;
}

void DiscordBot::UnregisterGlobalSlashCommand(const dpp::snowflake& snowflake, const std::string& command)
{
    if (GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) Querying Discord API to unregister global slash command '%s'...\n", GetConsolePrefix().c_str(), command.c_str());
    }

    m_BotCluster.global_commands_get([this, command, snowflake](const dpp::confirmation_callback_t& cb) {
        if (cb.is_error())
        {
            MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) ERROR: Failed to get global slash commands from Discord API when deleting '%s'\n", GetConsolePrefix().c_str(), command.c_str());
            MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) %s\n", GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
        }
        else
        {
            dpp::slashcommand_map cmdsMap = std::get<dpp::slashcommand_map>(cb.value);
            dpp::slashcommand_map::iterator it = cmdsMap.find(snowflake);

            if (it == cmdsMap.end())
            {
                MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) Command '%s' it's not registered in Discord API\n", GetConsolePrefix().c_str(), command.c_str());
                return;
            }

            m_BotCluster.global_command_delete(snowflake, [this, snowflake, command](const dpp::confirmation_callback_t& cb) {
                if (cb.is_error())
                {
                    MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) ERROR: Failed to delete global slash command '%s' from Discord API \n", GetConsolePrefix().c_str(), command.c_str());
                    MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) %s\n", GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
                }
                else
                {
                    if (GetLogLevel() != LogLevel::NONE)
                    {
                        MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) Slash command '%s' has been unregistered succesfully\n", command.c_str());

                        m_GlobalSlashCommands.erase(snowflake);
                    }
                }
             });
        }
    });
}

void DiscordBot::ClearGlobalSlashCommands()
{
    m_BotCluster.global_bulk_command_delete([this](const dpp::confirmation_callback_t& cb) {
        if (cb.is_error())
        {
            MF_PrintSrvConsole("%s (ClearGlobalSlashCommands) ERROR: Failed to clear global slash commands\n", GetConsolePrefix().c_str());
            MF_PrintSrvConsole("%s (ClearGlobalSlashCommands) %s\n", GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
        }
        else
        {
            m_GlobalSlashCommands.clear();

            if (GetLogLevel() == LogLevel::DEFAULT || GetLogLevel() == LogLevel::VERBOSE)
            {
                MF_PrintSrvConsole("%s (ClearGlobalSlashCommands) All global slash commands have been removed\n", GetConsolePrefix().c_str());
            }
        }
    });
}

void DiscordBot::SendMessageToChannel(const std::string& channel_id, const std::string& message)
{
    const dpp::snowflake channel(channel_id);

	dpp::message msg(channel, message);

	m_BotCluster.message_create(msg);
}

bool DiscordBot::SendReplyToLastInteraction(const std::string& message)
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
    m_Ready = state;

    if (GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s Bot has been marked as %sactive\n", GetConsolePrefix().c_str(), m_Ready ? "" : "in");
    }
}

void DiscordBot::RegisterEventsListeners()
{
    m_BotCluster.on_log([this](dpp::log_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (this == nullptr || !this->GetReadyState())
            {
                return;
            }
            if (cb.severity > dpp::loglevel::ll_info)
            {
                std::string level = cb.severity == dpp::loglevel::ll_critical ? "CRITICAL" : dpp::loglevel::ll_error ? "ERROR" : "WARNING";

                MF_PrintSrvConsole("\n----------------------------------\n");
                MF_PrintSrvConsole("%s %s log! Identifier: %s\n", GetConsolePrefix().c_str(), level.c_str(), m_Identifier.c_str());
                MF_PrintSrvConsole("%s JSON log: %s\n", GetConsolePrefix().c_str(), cb.message.c_str());
                MF_PrintSrvConsole("----------------------------------\n");
                return;
            }

            if (GetLogLevel() == LogLevel::NONE)
            {
                return;
            }

            if (GetLogLevel() == LogLevel::DEFAULT && cb.severity == dpp::loglevel::ll_info)
            {
                MF_PrintSrvConsole("%s %s\n", GetConsolePrefix().c_str(), cb.message.c_str());
            }

            if (GetLogLevel() == LogLevel::VERBOSE)
            {
                MF_PrintSrvConsole("%s %s\n", GetConsolePrefix().c_str(), cb.message.c_str());
            }
        });
    });

    m_BotCluster.on_ready([this](dpp::ready_t cb) {
        if (this == nullptr || !this->GetReadyState())
        {
            return;
        }

        m_BotCluster.global_commands_get([this](dpp::confirmation_callback_t cb) {
            if (this == nullptr || !this->GetReadyState())
            {
                return;
            }

            dpp::slashcommand_map cmdsMap;
            bool hadError = cb.is_error();
            std::string errorMessage;

            if (hadError) {
                errorMessage = cb.get_error().human_readable;
            }
            else {
                cmdsMap = std::get<dpp::slashcommand_map>(cb.value);
            }

            g_EventsQueue->Push([
                this,
                hadError,
                cmdsMap = std::move(cmdsMap),
                errorMessage = std::move(errorMessage)
            ]() {
                if (this == nullptr || !this->GetReadyState())
                {
                    return;
                }

                if (hadError)
                {
                    MF_PrintSrvConsole("%s ERROR: Failed to retrieve global slash commands from Discord API\n", GetConsolePrefix().c_str());
                    MF_PrintSrvConsole("%s %s\n", GetConsolePrefix().c_str(), errorMessage.c_str());
                }
                else
                {
                        
                    std::size_t slashCommandsCount = cmdsMap.size();

                    if (slashCommandsCount > 0)
                    {
                        for (const auto& [key, value] : cmdsMap)
                        {
                            m_GlobalSlashCommands.emplace(key, SlashCommand(value, INVALID_SLASH_COMMAND_AMXX_FW_HANDLE));
                        }

                        if (GetLogLevel() == LogLevel::VERBOSE)
                        {
                            MF_PrintSrvConsole("%s Retrieved %i global slash command%s from Discord API\n", GetConsolePrefix().c_str(), slashCommandsCount == 1 ? "s" : "");
                        }
                    }
                    else if (GetLogLevel() == LogLevel::VERBOSE)
                    {
                        MF_PrintSrvConsole("%s No global slash commands are registered for this bot on Discord API\n", GetConsolePrefix().c_str());
                    }

                    SetReadyState(true);
                    ExecuteForward(ON_BOT_READY, m_Identifier.c_str());
                }
            });
        });
    });

    m_BotCluster.on_slashcommand([this](dpp::slashcommand_t cb) {
        g_EventsQueue->Push([this, cb]() {
            if (this == nullptr || !this->GetReadyState())
            {
                return;
            }

            SlashCommandsMap::iterator it = m_GlobalSlashCommands.find(cb.command.get_command_interaction().id);

            if (it == m_GlobalSlashCommands.end())
            {
                return;
            }

            if (it->second.amx_fw_handle == INVALID_SLASH_COMMAND_AMXX_FW_HANDLE)
            {
                MF_PrintSrvConsole("%s Slash command issued by user '%s', but it does not exists in local slash commands map\n", GetConsolePrefix().c_str(), cb.command.usr.username);
                return;
            }

            m_CanSendInteractionMessage = true;

            if (m_Options.print_events_data || GetLogLevel() == LogLevel::VERBOSE)
            {
                MF_PrintSrvConsole("%s OnGlobalSlashCommand '%s': \n%s\n", GetConsolePrefix().c_str(), cb.command.get_command_interaction().name.c_str(), cb.command.to_json().dump(4).c_str());
            }

            MF_ExecuteForward(it->second.amx_fw_handle, cb.command.usr.build_json().c_str());

            if (!m_LastInteractionMessage.empty())
            {
                cb.reply(m_LastInteractionMessage);
                m_LastInteractionMessage.clear();
            }

            m_CanSendInteractionMessage = false;
        });
    });

    m_BotCluster.on_message_create([this](dpp::message_create_t cb) mutable {
        g_EventsQueue->Push([this, cb]() {
            if (this == nullptr || !this->GetReadyState())
            {
                return;
            }

            m_CanSendInteractionMessage = true;

            dpp::json eventData{};

            eventData["id"] = cb.msg.id.str();
            eventData["content"] = cb.msg.content;
            eventData["guild_id"] = cb.msg.guild_id.str();
            eventData["author"] =
            {
                { "id", cb.msg.author.id.str() },
                { "username", cb.msg.author.username }
            };

            eventData["mentions"] = dpp::json::array();

            for (const auto& pair : cb.msg.mentions)
            {
                const dpp::user& user = pair.first;

                eventData["mentions"].push_back({
                    { "id", user.id.str() },
                    { "username", user.username }
                });
            }

            if (m_Options.print_events_data || GetLogLevel() == LogLevel::VERBOSE)
            {
                MF_PrintSrvConsole("%s OnChannelMessageCreated: \n%s\n", GetConsolePrefix().c_str(), eventData.dump(4).c_str());
            }

            ExecuteForward(ON_CHANNEL_MESSAGE_CREATED, m_Identifier.c_str(), cb.msg.channel_id.str().c_str(), eventData.dump().c_str());

            if (!m_LastInteractionMessage.empty())
            {
                cb.reply(m_LastInteractionMessage);
                m_LastInteractionMessage.clear();
            }

            m_CanSendInteractionMessage = false;
        });
    });

    /*
    m_BotCluster.on_message_create([this](const dpp::message_create_t& cb) {
        m_CanSendInteractionMessage = true;

        dpp::json eventData{};

        eventData["id"] = cb.msg.id.str();
        eventData["content"] = cb.msg.content;
        eventData["guild_id"] = cb.msg.guild_id.str();
        eventData["author"] =
        {
            { "id", cb.msg.author.id.str() },
            { "username", cb.msg.author.username }
        };

        eventData["mentions"] = dpp::json::array();

        for (const auto& pair : cb.msg.mentions)
        {
            const dpp::user& user = pair.first;

            eventData["mentions"].push_back({
                { "id", user.id.str() },
                { "username", user.username }
            });
        }

        if (m_Options.print_events_data || GetLogLevel() == LogLevel::VERBOSE)
        {
            MF_PrintSrvConsole("%s OnChannelMessageCreated: \n%s\n", GetConsolePrefix().c_str(), eventData.dump(4).c_str());
        }

        ExecuteForward(ON_CHANNEL_MESSAGE_CREATED, m_Identifier.c_str(), cb.msg.channel_id.str().c_str(), eventData.dump().c_str());

        if (!m_LastInteractionMessage.empty())
        {
            cb.reply(m_LastInteractionMessage);
            m_LastInteractionMessage.clear();
        }

        m_CanSendInteractionMessage = false;
    });
    */
}