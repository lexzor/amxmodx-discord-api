#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include "discord_bot/discord_bot.h"

class DiscordBotsManager
{
public:
	using DiscordBotMap = std::unordered_map<std::string, std::unique_ptr<DiscordBot>>;
	using OnDiscordBotFunction = std::function<void(DiscordBot&)>;

	DiscordBotsManager() = default;
    ~DiscordBotsManager();

    DiscordBotsManager(const DiscordBotsManager&) = delete;
    DiscordBotsManager& operator=(const DiscordBotsManager&) = delete;

    DiscordBotsManager(DiscordBotsManager&&) = delete;
    DiscordBotsManager& operator=(DiscordBotsManager&&) = delete;

	bool InitializeBot(const std::string& identifier, const std::string& token);
	bool DeinitializeBot(const std::string& identifier);
	void ForEach(OnDiscordBotFunction function);
	
	[[nodiscard]] DiscordBot* GetBotRawPtrByIdentifier(const std::string& identifier);
	[[nodiscard]] const DiscordBotMap& GetDiscordBotsMap() const noexcept;

private:
	DiscordBotMap m_BotsMap;
};