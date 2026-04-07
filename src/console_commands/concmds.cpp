#include "concmds.h"
#include "console_command.h"

#include <unordered_map>
#include <array>

constexpr int CMD_COLUMN_WIDTH = 20;

const ConsoleCommand g_ConsoleCommands[] = {
    { "help",           "List module commands and their descriptions",              &concmd_help },
    { "botlist",        "List bots and their status",                               &concmd_botlist },
};

void RegisterConsoleCommands()
{
    REG_SVR_COMMAND(const_cast<char*>("dapi"), &concmd_dapi);
}

void concmd_dapi()
{
    int argc = CMD_ARGC();
    if(argc < 1)
    {
        gpMetaUtilFuncs->pfnLogConsole(PLID, "Unrecognized DiscordAPI command. Usage: dapi <subcommand>\n");
        concmd_help();
        return;
    }

    const char* subcmd = CMD_ARGV(1);
    
    for(const ConsoleCommand& concmd : g_ConsoleCommands)
    {
        if(strcmp(concmd.name, subcmd) == 0)
        {
            concmd.function();
            return;
        }
    }

    gpMetaUtilFuncs->pfnLogConsole(PLID, "Unrecognized DiscordAPI command. Usage: dapi <subcommand>\n");
    concmd_help();
}

void concmd_help()
{
    gpMetaUtilFuncs->pfnLogConsole(PLID, "List of DiscordAPI console commands\n");

    for(const ConsoleCommand& concmd : g_ConsoleCommands)
    {
        gpMetaUtilFuncs->pfnLogConsole(PLID, "  %-*s %s\n", CMD_COLUMN_WIDTH, concmd.name, concmd.description);
    }
}

void concmd_botlist()
{
    const DiscordBotsManager::DiscordBotMap& botMap = g_DiscordBotsManager->GetDiscordBotsMap();

    if(!botMap.size())
    {
        gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] No registered bots\n");
        return;
    }

    gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] %i registered bot%s\n", botMap.size(), (botMap.size() > 1 ? "s" : ""));
    gpMetaUtilFuncs->pfnLogConsole(PLID, "%-5s %-*s %-*s %-*s\n", "Index", CMD_COLUMN_WIDTH, "Identifier", CMD_COLUMN_WIDTH, "Console Prefix", CMD_COLUMN_WIDTH, "State");

    std::size_t iter = 0;
    for(const auto& [identifier, bot] : botMap)
    {
        gpMetaUtilFuncs->pfnLogConsole(PLID, "%-5zu %-*s %-*s %-*s\n", ++iter, CMD_COLUMN_WIDTH, bot->GetIdentifier().c_str(), CMD_COLUMN_WIDTH, bot->GetConsolePrefix().c_str(), CMD_COLUMN_WIDTH, (bot->IsStarted() ? "active" : "inactive"));
    }
}