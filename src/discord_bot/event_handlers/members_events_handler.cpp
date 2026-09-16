#include "members_events_handler.h"

#include "amxxmodule.h"
#include "discord_bot/discord_bot.h"
#include "mpsc/events_queue.h"
#include "amxx/amx_forwards.h"

MembersEventsHandler::MembersEventsHandler(DiscordBot* bot)
    : m_Bot(bot)
{
    RegisterListeners();
}

void MembersEventsHandler::RegisterListeners()
{
    m_Bot->GetCluster().on_guild_member_add([this](dpp::guild_member_add_t cb) {
		g_EventsQueue->Push([this, cb]() {
			if (m_Bot == nullptr)
				return;
			
			OnGuildMemberCreate(cb);
		});
	});
    m_Bot->GetCluster().on_guild_member_remove([this](dpp::guild_member_remove_t cb) {
		g_EventsQueue->Push([this, cb]() {
			if (m_Bot == nullptr)
				return;
			
			OnGuildMemberDelete(cb);
		});
	});
    m_Bot->GetCluster().on_guild_member_update([this](dpp::guild_member_update_t cb) {
		g_EventsQueue->Push([this, cb]() {
			if (m_Bot == nullptr)
				return;
			
			OnGuildMemberUpdate(cb);
		});
	});
}

void MembersEventsHandler::OnGuildMemberCreate(const dpp::guild_member_add_t& cb)
{
	const dpp::guild_member& member = cb.added;
	const dpp::user* user = member.get_user();

	const std::string userId = member.user_id.str();
	const std::string username = user != nullptr ? user->username : "";
	const std::string nickname = member.get_nickname();

	ExecuteForward(ON_GUILD_MEMBER_CREATE, m_Bot->GetIdentifier().c_str(), member.guild_id.str().c_str(), userId.c_str(), nickname.c_str(), username.c_str());
}

void MembersEventsHandler::OnGuildMemberDelete(const dpp::guild_member_remove_t& cb)
{
	const std::string userId = cb.removed.id.str();
	const std::string username = cb.removed.username;

	ExecuteForward(ON_GUILD_MEMBER_DELETE, m_Bot->GetIdentifier().c_str(), cb.guild_id.str().c_str(), userId.c_str(), username.c_str());
}

void MembersEventsHandler::OnGuildMemberUpdate(const dpp::guild_member_update_t& cb)
{
	const dpp::guild_member& member = cb.updated;

	ExecuteForward(ON_GUILD_MEMBER_UPDATE, m_Bot->GetIdentifier().c_str(), member.guild_id.str().c_str(), member.user_id.str().c_str(), member.get_user()->username.c_str());
}