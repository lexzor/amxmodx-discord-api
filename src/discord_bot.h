#pragma once

#include <dpp/dpp.h>

#include <string>
#include <functional>

#include "discord_bot_options.h"
#include "discord_bot_slash_command.h"

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
	void SendMessageToChannel			(const std::string& channel_id, const std::string& message);
	bool SendReplyToLastInteraction		(const std::string& message);
	void SetOptions						(const DiscordBotOptions& options);
	void SetEventsDataConsolePrinting	(const bool state);

	inline const std::string&		GetConsolePrefix()					const { return m_Options.chat_prefix; }
	inline const LogLevel			GetLogLevel()						const { return m_Options.log_level; }
	inline const SlashCommandsMap&	GetGlobalSlashCommandsMap()		    const { return m_GlobalSlashCommands; }
	inline const GuildsMap&			GetGuildsMap()						const { return m_Guilds; }
	inline const bool				GetReadyState()						const { return m_Ready; }

private:
	void							SetReadyState(bool state);
	void							RegisterEventsListeners();
	inline const std::string*		GetLastInteractionMessage()			const { return &m_LastInteractionMessage; }

private:
	dpp::cluster m_BotCluster;
	std::string m_Identifier;
	SlashCommandsMap m_GlobalSlashCommands;
	GuildsMap m_Guilds;

	DiscordBotOptions m_Options;

	bool m_Ready = false;
	bool m_ShouldPrintEventsData = false;
	bool m_CanSendInteractionMessage = false;
	std::string m_LastInteractionMessage;
};
