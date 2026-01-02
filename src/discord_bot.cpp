#include <amxxmodule.h>
#include "discord_bot.h"
#include "amx_forwards.h"
#include "dpp/nlohmann/json.hpp"

DiscordBot::DiscordBot(const std::string& identifier, const std::string& token)
    : m_BotCluster(token, dpp::i_default_intents | dpp::i_message_content, 1), m_Options(DiscordBotOptions()), m_Identifier(identifier)
{
    RegisterEventsListeners();
}

DiscordBot::~DiscordBot()
{
    m_BotCluster.shutdown();
}

void DiscordBot::SetOptions(const DiscordBotOptions& options)
{
    m_Options = options;
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

            MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) Slash command '%s' already registered.\n");
            return;
        }
    }

    m_BotCluster.global_command_create(
        { command, description, m_BotCluster.me.id },
        [this, command, amx_fw_handle](const dpp::confirmation_callback_t& cb) {
            if (cb.is_error())
            {
                MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) ERROR: Failed to register slash command '%s'\n", this->GetConsolePrefix().c_str(), command.c_str());
                MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) %s\n", this->GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
            }
            else
            {
                const dpp::slashcommand& createdSlashCmd = std::get<dpp::slashcommand>(cb.value);
                const dpp::snowflake snowflake = createdSlashCmd.id;

                m_GlobalSlashCommands.emplace(snowflake, SlashCommand(createdSlashCmd, amx_fw_handle));

                if (this->GetLogLevel() == LogLevel::DEFAULT || this->GetLogLevel() == LogLevel::VERBOSE)
                {
                    MF_PrintSrvConsole("%s (RegisterGlobalSlashCommand) Slash command '%s' has been registered succesfully\n", this->GetConsolePrefix().c_str(), command.c_str());
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
            MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) ERROR: Failed to get global slash commands from Discord API when deleting '%s'\n", this->GetConsolePrefix().c_str(), command.c_str());
            MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) %s\n", this->GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
        }
        else
        {
            dpp::slashcommand_map cmdsMap = std::get<dpp::slashcommand_map>(cb.value);
            dpp::slashcommand_map::iterator it = cmdsMap.find(snowflake);

            if (it == cmdsMap.end())
            {
                MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) Command '%s' it's not registered in Discord API\n", this->GetConsolePrefix().c_str(), command.c_str());
                return;
            }

            m_BotCluster.global_command_delete(snowflake, [this, snowflake, command](const dpp::confirmation_callback_t& cb) {
                if (cb.is_error())
                {
                    MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) ERROR: Failed to delete global slash command '%s' from Discord API \n", this->GetConsolePrefix().c_str(), command.c_str());
                    MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) %s\n", this->GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
                }
                else
                {
                    if (this->GetLogLevel() != LogLevel::NONE)
                    {
                        MF_PrintSrvConsole("%s (UnregisterGlobalSlashCommand) Slash command '%s' has been unregistered succesfully\n", command.c_str());

                        m_GlobalSlashCommands.erase(snowflake);
                    }
                }
             });
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
        MF_PrintSrvConsole("%s There is not active interaction which should receive a reply\n", GetConsolePrefix().c_str());
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
        MF_PrintSrvConsole("%s Bot hs been marked as %sactive\n", GetConsolePrefix().c_str(), m_Ready ? "" : "in");
    }
}

void DiscordBot::RegisterEventsListeners()
{
    m_BotCluster.on_log([this](const dpp::log_t& event) {
        if (event.severity > dpp::loglevel::ll_info)
        {
            std::string level = event.severity == dpp::loglevel::ll_critical ? "CRITICAL" : dpp::loglevel::ll_error ? "ERROR" : "WARNING";

            MF_PrintSrvConsole("\n----------------------------------\n");
            MF_PrintSrvConsole("%s %s log! Identifier: %s\n", this->GetConsolePrefix().c_str(), level.c_str(), m_Identifier.c_str());
            MF_PrintSrvConsole("%s JSON log: %s\n", this->GetConsolePrefix().c_str(), event.message.c_str());
            MF_PrintSrvConsole("----------------------------------\n");
            return;
        }

        if (GetLogLevel() == LogLevel::NONE)
        {
            return;
        }

        if (GetLogLevel() == LogLevel::DEFAULT && event.severity == dpp::loglevel::ll_info)
        {
            MF_PrintSrvConsole("%s %s\n", this->GetConsolePrefix().c_str(), event.message.c_str());
        }

        if (GetLogLevel() == LogLevel::VERBOSE)
        {
            MF_PrintSrvConsole("%s %s\n", this->GetConsolePrefix().c_str(), event.message.c_str());
        }
    });

    m_BotCluster.on_guild_create([this](const dpp::guild_create_t& cb) {
        m_Guilds.emplace(cb.created.id, cb.created);
        MF_PrintSrvConsole("%s Bot has been added in '%s'\n", this->GetConsolePrefix().c_str(), cb.created.name.c_str());
    });

    m_BotCluster.on_guild_delete([this](const dpp::guild_delete_t& cb) {
        if (cb.deleted.is_unavailable()) {
            MF_PrintSrvConsole("%s '%s' has become unavailable (temporarly)\n", this->GetConsolePrefix().c_str(), cb.deleted.name.c_str());
            m_Guilds[cb.deleted.id] = cb.deleted;
        }
        else
        {
            m_Guilds.erase(cb.deleted.id);
            MF_PrintSrvConsole("%s Bot was removed from '%s'\n", this->GetConsolePrefix().c_str(), cb.deleted.name.c_str());
        }
    });

    m_BotCluster.on_guild_update([this](const dpp::guild_update_t& cb) {
        m_Guilds[cb.updated.id] = cb.updated;

        if (this->GetLogLevel() == LogLevel::VERBOSE)
        {
            MF_PrintSrvConsole("%s Guild '%s' has been updated\n", this->GetConsolePrefix().c_str(), cb.updated.name.c_str());
        }
    });

    m_BotCluster.on_ready([this](const dpp::ready_t& event) {
        if (this->GetLogLevel() == LogLevel::VERBOSE)
        {
            MF_PrintSrvConsole("%s Fetching global slash commands from Discord API...\n", this->GetConsolePrefix().c_str());
        }

        m_BotCluster.global_commands_get([this](const dpp::confirmation_callback_t& cb) {
            if (cb.is_error())
            {
                MF_PrintSrvConsole("%s ERROR: Failed to retrieve global slash commands from Discord API\n", this->GetConsolePrefix().c_str());
                MF_PrintSrvConsole("%s %s\n", this->GetConsolePrefix().c_str(), cb.get_error().human_readable.c_str());
            }
            else
            {
                dpp::slashcommand_map cmdsMap = std::get<dpp::slashcommand_map>(cb.value);
                std::size_t slashCommandsCount = cmdsMap.size();

                if (slashCommandsCount > 0)
                {
                    for (const auto& [key, value] : cmdsMap)
                    {
                        m_GlobalSlashCommands.emplace(key, SlashCommand(value, INVALID_SLASH_COMMAND_AMXX_FW_HANDLE));
                    }

                    if (this->GetLogLevel() == LogLevel::VERBOSE)
                    {
                        MF_PrintSrvConsole("%s Retrieved %i global slash command%s from Discord API\n", this->GetConsolePrefix().c_str(), slashCommandsCount, slashCommandsCount == 1 ? "s" : "");
                    }
                }
                else if (this->GetLogLevel() == LogLevel::VERBOSE)
                {
                    MF_PrintSrvConsole("%s No global slash commands are registered for this bot on Discord API\n", this->GetConsolePrefix().c_str());
                }

                this->SetReadyState(true);
                ExecuteForward(ON_BOT_READY, m_Identifier.c_str());
            }
        });
    });

    m_BotCluster.on_slashcommand([this](const dpp::slashcommand_t& cb) {
        SlashCommandsMap::iterator it = m_GlobalSlashCommands.find(cb.command.get_command_interaction().id);

        if (it == m_GlobalSlashCommands.end())
        {
            return;
        }

        if (it->second.amx_fw_handle == INVALID_SLASH_COMMAND_AMXX_FW_HANDLE)
        {
            MF_PrintSrvConsole("%s Slash command issued by user %s, but it does not exists in local slash commands map\n", this->GetConsolePrefix().c_str(), cb.command.usr.username);
            return;
        }

        m_CanSendInteractionMessage = true;

        MF_ExecuteForward(it->second.amx_fw_handle, cb.command.usr.build_json().c_str());

        if (!m_LastInteractionMessage.empty())
        {
            cb.reply(m_LastInteractionMessage);
            m_LastInteractionMessage.clear();
        }

        m_CanSendInteractionMessage = false;
    });

    m_BotCluster.on_message_create([this](const dpp::message_create_t& cb) {
        m_CanSendInteractionMessage = true;

        nlohmann::json eventData{};

        eventData["id"] = cb.msg.id.str();
        eventData["content"] = cb.msg.content;
        eventData["guild_id"] = cb.msg.guild_id.str();
        eventData["author"] =
        {
            { "id", cb.msg.author.id.str() },
            { "username", cb.msg.author.username }
        };

        eventData["mentions"] = nlohmann::json::array();

        for (const auto& pair : cb.msg.mentions)
        {
            const dpp::user& user = pair.first;

            eventData["mentions"].push_back({
                { "id", user.id.str() },
                { "username", user.username }
            });
        }

        ExecuteForward(ON_CHANNEL_MESSAGE_CREATED, m_Identifier.c_str(), cb.msg.channel_id.str().c_str(), eventData.dump().c_str());

        if (!m_LastInteractionMessage.empty())
        {
            cb.reply(m_LastInteractionMessage);
            m_LastInteractionMessage.clear();
        }

        m_CanSendInteractionMessage = false;
    });
}