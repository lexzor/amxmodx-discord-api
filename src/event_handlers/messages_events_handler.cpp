#include "messages_events_handler.h"

#include "amxxmodule.h"
#include "discord_bot.h"
#include "events_queue.h"
#include "amx_forwards.h"

MessagesEventsHandler::MessagesEventsHandler(DiscordBot* bot)
	: m_Bot(bot)
{
	RegisterListeners();
}

MessagesEventsHandler::~MessagesEventsHandler()
{
    m_Bot = nullptr;
}

void MessagesEventsHandler::RegisterListeners()
{
	m_Bot->GetCluster().on_message_create([this](const dpp::message_create_t& cb) {
        g_EventsQueue->Push([this, cb]() {
            if (m_Bot == nullptr)
                return;
            
			OnMessageCreate(cb);
        });
	});
}

void MessagesEventsHandler::OnMessageCreate(const dpp::message_create_t& cb)
{
    dpp::json eventData{};

    eventData["id"] = cb.msg.id.str();
    eventData["content"] = cb.msg.content;
    eventData["guild_id"] = cb.msg.guild_id.str();
    eventData["author"] =
    {
        { "id", cb.msg.author.id.str() },
        { "username", cb.msg.author.username }
    };

    eventData["mentions"] = dpp::json::array();

    for (const auto& pair : cb.msg.mentions)
    {
        const dpp::user& user = pair.first;

        eventData["mentions"].push_back({
            { "id", user.id.str() },
            { "username", user.username }
            });
    }

    if (m_Bot->GetOptions().print_events_data || m_Bot->GetLogLevel() == LogLevel::VERBOSE)
    {
        MF_PrintSrvConsole("%s OnChannelMessageCreated: \n%s\n", m_Bot->GetConsolePrefix().c_str(), eventData.dump(4).c_str());
    }

    m_Bot->SetInteractionReplyAbility(true);
    ExecuteForward(ON_CHANNEL_MESSAGE_CREATED, m_Bot->GetIdentifier().c_str(), cb.msg.channel_id.str().c_str(), eventData.dump().c_str());
	m_Bot->SetInteractionReplyAbility(false);

	const std::string& lastInteractionMessage = m_Bot->GetInteractionMessage();

    if (!lastInteractionMessage.empty())
    {
        cb.reply(lastInteractionMessage.c_str());
        m_Bot->ClearInteractionMessage();
    }
}