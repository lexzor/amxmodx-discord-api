#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class ChannelsEventsHandler
{
public:
	ChannelsEventsHandler(DiscordBot* bot);
	~ChannelsEventsHandler();

	void RegisterListeners();

	void OnChannelCreate(const dpp::channel_create_t& cb);
	void OnChannelDelete(const dpp::channel_delete_t& cb);
	void OnChannelUpdate(const dpp::channel_update_t& cb);

private:
	DiscordBot* m_Bot;
};