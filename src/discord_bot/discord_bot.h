#pragma once

#include <dpp/dpp.h>

#include <string>
#include <functional>
#include <atomic>

#include "discord_bot/discord_bot_options.h"
#include "discord_bot/discord_bot_slash_command.h"
#include "event_handlers/guilds_events_handler.h"
#include "event_handlers/messages_events_handler.h"
#include "event_handlers/ready_event_handler.h"
#include "event_handlers/log_event_handler.h"

class DiscordBot
{
public:
	using SlashCommandsMap = std::unordered_map<dpp::snowflake, SlashCommand>;
	using GuildsMap = std::unordered_map<dpp::snowflake, dpp::guild>;

	DiscordBot(const std::string &identifier, const std::string &token);
	~DiscordBot() noexcept;

	bool Start();
	bool Stop();

	void SendMessageToChannel(const std::string &channel_id, const std::string &message);
	bool SendReplyToLastInteraction(const std::string &message);

	void SetOptions(const DiscordBotOptions &options);
	void SetEventsDataConsolePrinting(const bool state);
	void SetReadyState(bool state);

	[[nodiscard]] dpp::cluster &GetCluster() noexcept;
	[[nodiscard]] const std::string &GetConsolePrefix() const noexcept; 

	[[nodiscard]] const LogLevel GetLogLevel() const noexcept;

	[[nodiscard]] const bool IsStarted() const noexcept;
	[[nodiscard]] const std::string &GetIdentifier() const noexcept;
	[[nodiscard]] const DiscordBotOptions &GetOptions() const noexcept;
	[[nodiscard]] const std::string &GetInteractionMessage() const noexcept;

	[[nodiscard]] GuildsMap &GetGuildsMap() noexcept;
	[[nodiscard]] SlashCommandsMap &GetGlobalSlashCommandsMap() noexcept;

	void SetInteractionReplyAbility(bool state);
	void SetInteractionMessage(const std::string &message);
	void ClearInteractionMessage();

private:
	[[nodiscard]] const std::string *GetLastInteractionMessage() const noexcept;
	[[nodiscard]] const bool IsDestroyed() const noexcept;

private:
	dpp::cluster m_BotCluster;
	std::string m_Identifier;
	SlashCommandsMap m_GlobalSlashCommands;
	GuildsMap m_Guilds;

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
};