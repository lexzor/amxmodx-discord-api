#include "channels_events_handler.h"

#include "amxxmodule.h"
#include "discord_bot/discord_bot.h"
#include "mpsc/events_queue.h"
#include "amxx/amx_forwards.h"

ChannelsEventsHandler::ChannelsEventsHandler(DiscordBot* bot)
    : m_Bot(bot)
{
    RegisterListeners();
}

ChannelsEventsHandler::~ChannelsEventsHandler()
{
    m_Bot = nullptr;
}

void ChannelsEventsHandler::RegisterListeners()
{
    m_Bot->GetCluster().on_channel_create([this](dpp::channel_create_t cb) {
        dpp::channel* newCached = new dpp::channel(cb.created);
        dpp::get_channel_cache()->store(newCached);
        
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;

            OnChannelCreate(cb);
        });
    });

    m_Bot->GetCluster().on_channel_delete([this](dpp::channel_delete_t cb) {
        dpp::channel* cached = dpp::find_channel(cb.deleted.id);
        if (cached)
            dpp::get_channel_cache()->remove(cached);

        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;

            OnChannelDelete(cb);
        });
    });

    m_Bot->GetCluster().on_channel_update([this](dpp::channel_update_t cb) {
        dpp::channel* cached = dpp::find_channel(cb.updated.id);
        if (cached)
            *cached = cb.updated;
        else
        {
            dpp::channel* newCached = new dpp::channel(cb.updated);
            dpp::get_channel_cache()->store(newCached);
        }

        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;

            OnChannelUpdate(cb);
        });
    });
}

void ChannelsEventsHandler::OnChannelCreate(const dpp::channel_create_t& cb)
{
    ExecuteForward(ON_GUILD_CHANNEL_CREATE, m_Bot->GetIdentifier().c_str(), -1, true, cb.created.id.str().c_str());
}

void ChannelsEventsHandler::OnChannelDelete(const dpp::channel_delete_t& cb)
{

    ExecuteForward(ON_GUILD_CHANNEL_DELETE, m_Bot->GetIdentifier().c_str(), true, cb.deleted.id.str().c_str());
}

void ChannelsEventsHandler::OnChannelUpdate(const dpp::channel_update_t& cb)
{
    ExecuteForward(ON_GUILD_CHANNEL_EDIT, m_Bot->GetIdentifier().c_str(), -1, true, cb.updated.id.str().c_str());
}