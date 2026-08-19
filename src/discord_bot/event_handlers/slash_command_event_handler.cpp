#include "slash_command_event_handler.h"

#include "amxxmodule.h"
#include "discord_bot/discord_bot.h"
#include "mpsc/events_queue.h"
#include "amxx/amx_forwards.h"

SlashCommandEventHandler::SlashCommandEventHandler(DiscordBot* bot)
	: m_Bot(bot)
{
	RegisterListeners();
}

SlashCommandEventHandler::~SlashCommandEventHandler()
{
	m_Bot = nullptr;
}

void SlashCommandEventHandler::RegisterListeners()
{
	m_Bot->GetCluster().on_slashcommand([this](dpp::slashcommand_t cb) {
        cb.thinking();

		g_EventsQueue->Push([this, cb]() {
			if (m_Bot == nullptr)
				return;

			OnSlashCommand(cb);
		});
	});
}

void SlashCommandEventHandler::OnSlashCommand(const dpp::slashcommand_t cb)
{
    nlohmann::json paramsJson;

    for (const dpp::command_data_option& opt : cb.command.get_command_interaction().options)
    {
        std::visit([&](auto&& val) {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::string>)
                paramsJson[opt.name] = val;
            else if constexpr (std::is_same_v<T, int64_t>)
                paramsJson[opt.name] = val;
            else if constexpr (std::is_same_v<T, double>)
                paramsJson[opt.name] = val;
            else if constexpr (std::is_same_v<T, dpp::snowflake>)
                paramsJson[opt.name] = val.str();
            else if constexpr (std::is_same_v<T, std::monostate>)
                paramsJson[opt.name] = nullptr;
            }, opt.value);
    }

    const std::string paramsJsonStr = paramsJson.dump();

    m_Bot->SetInteractionReplyAbility(true);
    ExecuteForward(ON_GUILD_SLASH_COMMAND,
        m_Bot->GetIdentifier().c_str(), // bot identifier
        cb.command.guild_id.str().c_str(), // guild id
        cb.command.get_command_name().c_str(), // command name
        cb.command.usr.id.str().c_str(), // user id
        cb.command.usr.username.c_str(), // username
        paramsJsonStr.c_str(), // options
        cb.command.channel_id.str().c_str() // channelid
    );
    m_Bot->SetInteractionReplyAbility(false);

    const std::string& lastInteractionMessage = m_Bot->GetInteractionMessage();

    if (!lastInteractionMessage.empty())
    {
        cb.reply(lastInteractionMessage.c_str());
        m_Bot->ClearInteractionMessage();
    }
    else
    {
        cb.reply("Bot failed to send a response");
    }
}