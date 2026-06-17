#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class SlashCommandEventHandler
{
public:
	SlashCommandEventHandler(DiscordBot* bot);
	~SlashCommandEventHandler();

	void RegisterListeners();

	void OnSlashCommand(const dpp::slashcommand_t cb);

private:
	DiscordBot* m_Bot;
};