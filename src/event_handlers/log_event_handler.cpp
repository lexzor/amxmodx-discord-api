#include "log_event_handler.h"

#include "amxxmodule.h"
#include "discord_bot.h"
#include "events_queue.h"
#include "amx_forwards.h"

LogEventHandler::LogEventHandler(DiscordBot* bot)
	: m_Bot(bot)
{
	RegisterListeners();
}

LogEventHandler::~LogEventHandler()
{
	m_Bot = nullptr;
}

void LogEventHandler::RegisterListeners()
{
	m_Bot->GetCluster().on_log([this](dpp::log_t cb) {
		g_EventsQueue->Push([this, cb]() {
			if (m_Bot == nullptr)
				return;
			
			OnLog(cb);
		});
	});
}

void LogEventHandler::OnLog(const dpp::log_t& cb)
{
    if (cb.severity > dpp::loglevel::ll_info)
    {
        std::string level = cb.severity == dpp::loglevel::ll_critical ? "CRITICAL" : cb.severity == dpp::loglevel::ll_error ? "ERROR" : "WARNING";

        MF_PrintSrvConsole("\n----------------------------------\n");
        MF_PrintSrvConsole("%s %s log! Identifier: %s\n", m_Bot->GetConsolePrefix().c_str(), level.c_str(), m_Bot->GetIdentifier().c_str());
        MF_PrintSrvConsole("%s JSON log: %s\n", m_Bot->GetConsolePrefix().c_str(), cb.message.c_str());
        MF_PrintSrvConsole("----------------------------------\n");
        return;
    }

    if (m_Bot->GetLogLevel() == LogLevel::NONE)
    {
        return;
    }

    if (m_Bot->GetLogLevel() == LogLevel::DEFAULT && cb.severity == dpp::loglevel::ll_info)
    {
        MF_PrintSrvConsole("%s %s\n", m_Bot->GetConsolePrefix().c_str(), cb.message.c_str());
    }

    if (m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s %s\n", m_Bot->GetConsolePrefix().c_str(), cb.message.c_str());
    }
}