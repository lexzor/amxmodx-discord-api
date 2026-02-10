#include "ready_event_handler.h"

#include "amxxmodule.h"
#include "discord_bot.h"
#include "events_queue.h"
#include "amx_forwards.h"

ReadyEventHandler::ReadyEventHandler(DiscordBot* bot)
	: m_Bot(bot)
{
    RegisterListeners();
}

ReadyEventHandler::~ReadyEventHandler()
{
	m_Bot = nullptr;
}

void ReadyEventHandler::RegisterListeners()
{
	m_Bot->GetCluster().on_ready([this](const dpp::ready_t& cb) {
		g_EventsQueue->Push([this, cb]() {
			if (m_Bot == nullptr)
				return;
			
			OnReady();
		});
	});
}

void ReadyEventHandler::OnReady()
{
    m_Bot->SetReadyState(true);
    ExecuteForward(ON_BOT_READY, m_Bot->GetIdentifier().c_str());

    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s (OnBotReady) Querying Discord API to retrieve global shlash commands '%s'...\n", m_Bot->GetConsolePrefix().c_str());
    }

    m_Bot->GetCluster().global_commands_get([this](dpp::confirmation_callback_t cb) {
        if (m_Bot == nullptr)
            return;

        if (cb.is_error())
        {
            const std::string errorMessage = cb.get_error().human_readable;

            g_EventsQueue->Push([this, errorMessage]() {
                if (m_Bot == nullptr)
                    return;

                MF_PrintSrvConsole("%s ERROR: Failed to retrieve global slash commands from Discord API\n", m_Bot->GetConsolePrefix().c_str());
                MF_PrintSrvConsole("%s %s\n", m_Bot->GetConsolePrefix().c_str(), errorMessage.c_str());
            });
        }
        else
        {
            dpp::slashcommand_map cmdsMap;
            cmdsMap = std::get<dpp::slashcommand_map>(cb.value);

            g_EventsQueue->Push([this, cmdsMap]() {
                if (m_Bot == nullptr)
                    return;

                std::size_t slashCommandsCount = cmdsMap.size();

                if (slashCommandsCount > 0)
                {
                    for (const auto& [key, value] : cmdsMap)
                    {
						auto& botCmdsMap= m_Bot->GetGlobalSlashCommandsMap();
                        botCmdsMap.emplace(key, SlashCommand(value, INVALID_SLASH_COMMAND_AMXX_FW_HANDLE));
                    }

                    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
                    {
                        MF_PrintSrvConsole("%s Retrieved %i global slash command%s from Discord API\n", m_Bot->GetConsolePrefix().c_str(), slashCommandsCount, slashCommandsCount == 1 ? "s" : "");
                    }
                }
                else if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
                {
                    MF_PrintSrvConsole("%s No global slash commands are registered for this bot on Discord API\n", m_Bot->GetConsolePrefix().c_str());
                }
            });
        }
    });
}