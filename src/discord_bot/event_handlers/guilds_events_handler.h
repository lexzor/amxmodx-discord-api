#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class GuildsEventsHandler
{
public:
	GuildsEventsHandler(DiscordBot* bot);
	~GuildsEventsHandler();
	
	void RegisterListeners();

	void OnGuildCreate(const dpp::guild_create_t& cb);
	void OnGuildDelete(const dpp::guild_delete_t& cb);
	void OnGuildUpdate(const dpp::guild_update_t& cb);

private:
	DiscordBot* m_Bot;
};