#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class MessagesEventsHandler
{
public:
	MessagesEventsHandler(DiscordBot* bot);

	void RegisterListeners();

	void OnMessageCreate(const dpp::message_create_t& cb);

private:
	DiscordBot* m_Bot;
};