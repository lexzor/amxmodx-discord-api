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
		: log_level(LogLevel::DEFAULT), chat_prefix("[DiscordBot]") {}
	
	DiscordBotOptions(LogLevel _log_level, const std::string& _chat_prefix)
		: log_level(_log_level), chat_prefix(_chat_prefix) {}

	LogLevel log_level;
	std::string chat_prefix;
};