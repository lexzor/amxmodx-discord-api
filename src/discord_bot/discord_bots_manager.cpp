#include "discord_bots_manager.h"

DiscordBotsManager::~DiscordBotsManager()
{
	for (auto& pair : m_BotsMap)
		pair.second->Stop();
}

bool DiscordBotsManager::InitializeBot(const std::string& identifier, const std::string& token)
{
	DiscordBotMap::iterator it = m_BotsMap.find(identifier);

	if (it != m_BotsMap.end())
		return true;

	m_BotsMap.emplace(identifier, std::make_unique<DiscordBot>(identifier, token));

	return true;
}

bool DiscordBotsManager::DeinitializeBot(const std::string& identifier)
{
	return m_BotsMap.erase(identifier) > 0;
}

DiscordBot* DiscordBotsManager::GetBotRawPtrByIdentifier(const std::string& identifier)
{
	DiscordBotMap::iterator it = m_BotsMap.find(identifier);

	if (it == m_BotsMap.end())
		return nullptr;

	return it->second.get();
}

const DiscordBotsManager::DiscordBotMap& DiscordBotsManager::GetDiscordBotsMap() const noexcept
{
	return m_BotsMap;
}

void DiscordBotsManager::ForEach(OnDiscordBotFunction function)
{
	for(const auto& [identifier, bot] : m_BotsMap)
		function(*bot);
}