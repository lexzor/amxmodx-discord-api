#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class MembersEventsHandler
{
public:
	MembersEventsHandler(DiscordBot* bot);
	~MembersEventsHandler();

	void RegisterListeners();

	void OnGuildMemberCreate(const dpp::guild_member_add_t& cb);
	void OnGuildMemberDelete(const dpp::guild_member_remove_t& cb);
	void OnGuildMemberUpdate(const dpp::guild_member_update_t& cb);

private:
	DiscordBot* m_Bot;
};