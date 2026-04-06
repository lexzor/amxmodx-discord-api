#pragma once

#include <unordered_map>
#include <string>

#include "discord_bot.h"

class DiscordBotsManager
{
public:
	using DiscordBotMap = std::unordered_map<std::string, std::unique_ptr<DiscordBot>>;
	
	DiscordBotsManager() = default;
    ~DiscordBotsManager();

    DiscordBotsManager(const DiscordBotsManager&) = delete;
    DiscordBotsManager& operator=(const DiscordBotsManager&) = delete;

    DiscordBotsManager(DiscordBotsManager&&) = delete;
    DiscordBotsManager& operator=(DiscordBotsManager&&) = delete;

	bool InitializeBot(const std::string& identifier, const std::string& token);
	bool DeinitializeBot(const std::string& identifier);
	
	[[nodiscard]] DiscordBot* GetBotRawPtrByIdentifier(const std::string& identifier);
	[[nodiscard]] const DiscordBotMap& GetDiscordBotsMap() const;

private:
	DiscordBotMap m_BotsMap;
};

extern std::unique_ptr<DiscordBotsManager> g_DiscordBotsManager;