#pragma once

#include <amxxmodule.h>
#include "../discord_bot_options.h"
#include "../common.h"

static inline DiscordBotOptions ParseBotOptionsFromAmxArray(const cell* options)
{
	enum AmxxBotOptionsArray
	{
		LOG_LEVEL = 0,
		CONSOLE_PREFIX, // size = MAX_CONSOLE_PREFIX_LENGTH
		PRINT_EVENTS_DATA
	};

	uint16_t currentIter = AmxxBotOptionsArray::LOG_LEVEL;
	const LogLevel logLevel = static_cast<LogLevel>(options[currentIter]);
	
	currentIter = AmxxBotOptionsArray::CONSOLE_PREFIX;
	
	std::string chat_prefix;
	chat_prefix.reserve(MAX_CONSOLE_PREFIX_LENGTH);

	for (uint16_t i = 0; i < MAX_CONSOLE_PREFIX_LENGTH; i++)
	{
		char c = static_cast<char>(options[currentIter + i]);
		
		if (c == 0x0)
		{
			break;
		}

		chat_prefix.push_back(c);
	}

	if (chat_prefix.empty())
	{
		chat_prefix = "[DiscordBot]";
	}

	currentIter += MAX_CONSOLE_PREFIX_LENGTH;
	const bool print_events_data = options[currentIter] != 0;

	return DiscordBotOptions(logLevel, chat_prefix, print_events_data);
}