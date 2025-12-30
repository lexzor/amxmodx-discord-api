#include <amxmodx>
#include "./include/discordbot.inc"

new const DISCORD_BOT_IDENTIFIER[] = "discbot"
new const DISCORD_BOT_TOKEN[] = "MTQzNTQwMzQzMDA4MzQzMjU2OQ.GG-au2.zP3pJxaKf_Js55_qaR1za6iWhcij9novzSbB4s"

public plugin_init()
{
    register_plugin("Discord Bot", "0.1", "lexzor");

    new BotHandleKey:t = CreateBot(DISCORD_BOT_IDENTIFIER, DISCORD_BOT_TOKEN);

    server_print("Bot %i", _:t);
}