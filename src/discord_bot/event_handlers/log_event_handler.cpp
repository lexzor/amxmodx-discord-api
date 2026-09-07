#include "log_event_handler.h"

#include "amxxmodule.h"
#include "discord_bot/discord_bot.h"
#include "mpsc/events_queue.h"
#include "amxx/amx_forwards.h"

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
    bool shouldNotify;

    if (cb.severity > dpp::loglevel::ll_info)
    {
        std::string level = cb.severity == dpp::loglevel::ll_critical ? "CRITICAL" : cb.severity == dpp::loglevel::ll_error ? "ERROR" : "WARNING";

        MF_PrintSrvConsole("\n----------------------------------\n");
        MF_PrintSrvConsole("%s %s log! Identifier: %s\n", m_Bot->GetConsolePrefix().c_str(), level.c_str(), m_Bot->GetIdentifier().c_str());
        MF_PrintSrvConsole("%s Message log: %s\n", m_Bot->GetConsolePrefix().c_str(), cb.message.c_str());
        MF_PrintSrvConsole("%s Raw Event: %s\n", m_Bot->GetConsolePrefix().c_str(), cb.raw_event.c_str());
        MF_PrintSrvConsole("----------------------------------\n");

        shouldNotify = true;
    }
    else
    {
        const LogLevel logLevel = m_Bot->GetLogLevel();

        shouldNotify = (logLevel == LogLevel::DEFAULT && cb.severity == dpp::loglevel::ll_info) || logLevel == LogLevel::VERBOSE;

        if (shouldNotify)
            MF_PrintSrvConsole("%s %s\n", m_Bot->GetConsolePrefix().c_str(), cb.message.c_str());
    }

    if (shouldNotify)
        ExecuteForward(ON_BOT_LOG, m_Bot->GetIdentifier().c_str(), static_cast<int>(cb.severity), cb.message.c_str());
}