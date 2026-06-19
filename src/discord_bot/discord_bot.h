#pragma once

#include <dpp/dpp.h>

#include <string>
#include <functional>
#include <atomic>
#include <unordered_set>

#include "discord_bot/discord_bot_options.h"
#include "event_handlers/guilds_events_handler.h"
#include "event_handlers/messages_events_handler.h"
#include "event_handlers/ready_event_handler.h"
#include "event_handlers/log_event_handler.h"
#include "event_handlers/slash_command_event_handler.h"
#include "event_handlers/channels_events_handler.h"

class DiscordBot
{
public:
	using GlobalSlashCommandsMap = std::unordered_map<dpp::snowflake, dpp::slashcommand>;
	using GuildSlashCommandsMap = std::unordered_map<dpp::snowflake, dpp::slashcommand_map>;
	using GuildsSet = std::unordered_set<dpp::snowflake>;

	DiscordBot(const std::string &identifier, const std::string &token);
	~DiscordBot() noexcept;

	bool Start();
	bool Stop();

	void SendMessageToChannel(const std::string &channel_id, const std::string &message);
	bool SendReplyToLastInteraction(const std::string &message);

	void SetOptions(const DiscordBotOptions &options);
	void SetEventsDataConsolePrinting(const bool state);
	void SetReadyState(bool state);

	[[nodiscard]] dpp::cluster& GetCluster() noexcept;
	[[nodiscard]] const std::string& GetConsolePrefix() const noexcept; 

	[[nodiscard]] const LogLevel GetLogLevel() const noexcept;

	[[nodiscard]] const bool IsStarted() const noexcept;
	[[nodiscard]] const std::string& GetIdentifier() const noexcept;
	[[nodiscard]] const DiscordBotOptions& GetOptions() const noexcept;
	[[nodiscard]] const std::string& GetInteractionMessage() const noexcept;

	[[nodiscard]] GuildsSet& GetGuildsSet() noexcept;
	[[nodiscard]] GlobalSlashCommandsMap& GetGlobalSlashCommandsMap() noexcept;
	[[nodiscard]] GuildSlashCommandsMap& GetGuildsSlashCommandsMap() noexcept;

	void SetInteractionReplyAbility(bool state);
	void SetInteractionMessage(const std::string &message);
	void ClearInteractionMessage();

private:
	[[nodiscard]] const std::string *GetLastInteractionMessage() const noexcept;
	[[nodiscard]] const bool IsDestroyed() const noexcept;

private:
	dpp::cluster m_BotCluster;
	std::string m_Identifier {};
	GlobalSlashCommandsMap m_GlobalSlashCommands{};
	GuildSlashCommandsMap m_GuildSlashCommands{};
	GuildsSet m_Guilds {};

	DiscordBotOptions m_Options {};

	std::atomic<bool> m_IsDestroyed = false;
	bool m_Starting = false;
	bool m_Ready = false;
	bool m_ShouldPrintEventsData = false;
	bool m_CanSendInteractionMessage = false;
	std::string m_LastInteractionMessage {};

	std::unique_ptr<LogEventHandler> m_LogEventHandler {};
	std::unique_ptr<ReadyEventHandler> m_ReadyEventHandler {};
	std::unique_ptr<GuildsEventsHandler> m_GuildEventsHandler {};
	std::unique_ptr<MessagesEventsHandler> m_MessagesEventsHandler {};
	std::unique_ptr<SlashCommandEventHandler> m_SlashCommandEventHandler {};
	std::unique_ptr<ChannelsEventsHandler> m_ChannelsEventsHandlers {};
};