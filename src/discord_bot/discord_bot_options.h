#pragma once

#include <string>

enum LogLevel
{
	NONE = 0,
	DEFAULT,
	VERBOSE
};

struct DiscordBotOptions
{
public:
	DiscordBotOptions()
		: log_level(LogLevel::DEFAULT), chat_prefix("[DiscordBot]"), print_events_data(false) {}
	
	DiscordBotOptions(LogLevel _log_level, const std::string& _chat_prefix, bool _print_events_data)
		: log_level(_log_level), chat_prefix(_chat_prefix), print_events_data(_print_events_data) {}

	LogLevel log_level;
	std::string chat_prefix;
	bool print_events_data = false;
};