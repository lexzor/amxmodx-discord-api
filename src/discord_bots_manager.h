#pragma once

#include <unordered_map>
#include <string>

#include "utils/singleton.h"
#include "discord_bot.h"

class DiscordBotsManager : public Singleton<DiscordBotsManager>
{
public:
	~DiscordBotsManager();

	bool				InitializeBot(const std::string& identifier, const char* token);
	bool				DeinitializeBot(const std::string& identifier);
	DiscordBot*			GetBotRawPtrByIdentifier(const std::string& identifier);

private:
	using DiscordBotMap = std::unordered_map<std::string, std::unique_ptr<DiscordBot>>;

	DiscordBotMap m_BotsMap;
};