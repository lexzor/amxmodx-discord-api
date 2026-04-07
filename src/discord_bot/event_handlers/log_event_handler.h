#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class LogEventHandler
{
public:
	LogEventHandler(DiscordBot* bot);
	~LogEventHandler();

	void RegisterListeners();

	void OnLog(const dpp::log_t& cb);

private:
	DiscordBot* m_Bot;
};