#pragma once

#include <dpp/dpp.h>

#include <string>
#include <functional>
#include <atomic>

#include "discord_bot_options.h"
#include "discord_bot_slash_command.h"
#include "event_handlers/guilds_events_handler.h"
#include "event_handlers/messages_events_handler.h"
#include "event_handlers/ready_event_handler.h"

class DiscordBot
{
public:
	using SlashCommandsMap			= std::unordered_map<dpp::snowflake, SlashCommand>;
	using GuildsMap					= std::unordered_map<dpp::snowflake, dpp::guild>;

	DiscordBot(const std::string& identifier, const std::string& token);
	~DiscordBot();

	bool Start();
	bool Stop();

	void RegisterGlobalSlashCommand		(const std::string& command, const std::string& description, int amx_fw_handle);
	void UnregisterGlobalSlashCommand	(const dpp::snowflake& snowflake, const std::string& command);
	void ClearGlobalSlashCommands		();

	void SendMessageToChannel			(const std::string& channel_id, const std::string& message);
	bool SendReplyToLastInteraction		(const std::string& message);
	
	void SetOptions						(const DiscordBotOptions& options);
	void SetEventsDataConsolePrinting	(const bool state);
	void SetReadyState					(bool state);

	inline dpp::cluster&			GetCluster() { return m_BotCluster; }
	inline const std::string&		GetConsolePrefix()					const { return m_Options.chat_prefix; }
	inline const LogLevel			GetLogLevel()						const { return m_Options.log_level; }
	inline const bool				GetReadyState()						const { return m_Ready; }
	inline const std::string&		GetIdentifier()						const { return m_Identifier; }
	inline const DiscordBotOptions& GetOptions()						const { return m_Options; }
	inline const std::string&		GetInteractionMessage()				const { return m_LastInteractionMessage; }
	
	inline GuildsMap&				GetGuildsMap()							  { return m_Guilds; }
	inline SlashCommandsMap&		GetGlobalSlashCommandsMap()				  { return m_GlobalSlashCommands; }

	inline void                     SetInteractionReplyAbility(bool state)    { m_CanSendInteractionMessage = state; }
	inline void                     SetInteractionMessage(const std::string& message) { m_LastInteractionMessage = message; }
	inline void						ClearInteractionMessage()				  { m_LastInteractionMessage.clear(); }

private:
	//void							RegisterEventsListeners();
	inline const std::string*		GetLastInteractionMessage()			const { return &m_LastInteractionMessage; }
	inline const bool				IsDestroyed()						const { return m_IsDestroyed; }

private:
	dpp::cluster m_BotCluster;
	std::string m_Identifier;
	SlashCommandsMap m_GlobalSlashCommands;
	GuildsMap m_Guilds;

	DiscordBotOptions m_Options;

	std::atomic<bool> m_IsDestroyed		= false;
	bool m_Ready						= false;
	bool m_ShouldPrintEventsData		= false;
	bool m_CanSendInteractionMessage	= false;
	std::string m_LastInteractionMessage;

	std::unique_ptr<GuildsEventsHandler> m_GuildEventsHandler;
	std::unique_ptr<MessagesEventsHandler> m_MessagesEventsHandler;
	std::unique_ptr<ReadyEventHandler> m_ReadyEventHandler;
};