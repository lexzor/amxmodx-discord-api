#pragma once

#include <unordered_map>
#include <string>

#include "utils/singleton.h"
#include "discord_bot.h"

class DiscordBotsManager : public Singleton<DiscordBotsManager>
{
	using DiscordBotMap = std::unordered_map<std::string, std::unique_ptr<DiscordBot>>;
public:
	~DiscordBotsManager();

	bool				InitializeBot(const std::string& identifier, const char* token);
	bool				DeinitializeBot(const std::string& identifier);
	DiscordBot*			GetBotRawPtrByIdentifier(const std::string& identifier);
	DiscordBotMap&		GetDiscordBotsMap();

private:

	DiscordBotMap m_BotsMap;
};