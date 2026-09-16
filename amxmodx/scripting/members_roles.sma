#include <amxmodx>
#include <discordapi>

#define IDENTIFIER "discord_bot"
#define GUILD_ID "1534568445176320161"

public plugin_init()
{
    register_plugin("[DiscordAPI] Member Role", "0.1", "lexzor");

    // members
    register_clcmd("discord_member_exists_by_id", "cmd_discord_member_exists_by_id");
    register_clcmd("discord_member_exists_by_username", "cmd_discord_member_exists_by_username");
    register_clcmd("discord_get_member_id_by_username", "cmd_discord_get_member_id_by_username");
    register_clcmd("discord_get_member_string_by_id", "cmd_discord_get_member_string_by_id");
    register_clcmd("discord_get_member_int_by_id", "cmd_discord_get_member_int_by_id");
    register_clcmd("discord_get_members_count", "cmd_discord_get_members_count");
    register_clcmd("discord_get_member_string_by_index", "cmd_discord_get_member_string_by_index");
    register_clcmd("discord_get_member_int_by_index", "cmd_discord_get_member_int_by_index");
    register_clcmd("discord_list_members", "cmd_discord_list_members");
    register_clcmd("discord_member_has_role", "cmd_discord_member_has_role");
    register_clcmd("discord_add_member_role", "cmd_discord_add_member_role");
    register_clcmd("discord_remove_member_role", "cmd_discord_remove_member_role");
    register_clcmd("discord_fetch_member_by_id", "cmd_discord_fetch_member_by_id");
    register_clcmd("discord_fetch_member_by_username", "cmd_discord_fetch_member_by_username");

    //roles
    register_clcmd("discord_role_exists_by_id", "cmd_discord_role_exists_by_id");
    register_clcmd("discord_role_exists_by_name", "cmd_discord_role_exists_by_name");
    register_clcmd("discord_get_role_id_by_name", "cmd_discord_get_role_id_by_name");
}

public OnGuildMemberFetch(const identifier[], const guild_id[], const bool:success, const user_id[], const username[], const nickname[], const display_name[])
{
    server_print("[%s] OnGuildMemberFetched: guild %s, success %s, user_id %s, username %s, nickname %s, display_name %s", identifier, guild_id, success ? "true" : "false", user_id, username, nickname, display_name);
}

public OnGuildMemberCreate(const identifier[], const guild_id[], const user_id[], const nickname[], const username[])
{
    server_print("[%s] OnGuildMemberCreate: guild %s, user_id %s, nickname %s, username %s", identifier, guild_id, user_id, nickname, username);
}

public OnGuildMemberDelete(const identifier[], const guild_id[], const user_id[], const username[])
{
    server_print("[%s] OnGuildMemberDelete: guild %s, user_id %s, username %s", identifier, guild_id, user_id, username);
}

public OnGuildMemberUpdate(const identifier[], const guild_id[], const user_id[])
{
    server_print("[%s] OnGuildMemberUpdate: guild %s, user_id %s", identifier, guild_id, user_id);
}

public OnGuildMemberRoleAdd(const identifier[], const guild_id[], const user_id[], const role_id[], const bool:success)
{
    server_print("[%s] OnGuildMemberRoleAdd: guild %s, user_id %s, role_id %s, success %s", identifier, guild_id, user_id, role_id, success ? "true" : "false");
}

public OnGuildMemberRoleRemove(const identifier[], const guild_id[], const user_id[], const role_id[], const bool:success)
{
    server_print("[%s] OnGuildMemberRoleAdd: guild %s, user_id %s, role_id %s, success %s", identifier, guild_id, user_id, role_id, success ? "true" : "false");
}

public cmd_discord_member_exists_by_id(id)
{
    new memberId[32];
    read_argv(1, memberId, charsmax(memberId));

    server_print("Checking if member with id %s exists", memberId);

    new const bool:exists = GuildMemberExistsById(IDENTIFIER, GUILD_ID, memberId);

    server_print("Member %s", exists ? "exists" : "does not exist");
}

public cmd_discord_member_exists_by_username(id)
{
    new memberUsername[32];
    read_argv(1, memberUsername, charsmax(memberUsername));

    server_print("Checking if member with username %s exists", memberUsername);

    new const bool:exists = GuildMemberExistsByUsername(IDENTIFIER, GUILD_ID, memberUsername);

    server_print("Member %s", exists ? "exists" : "does not exist");
}

public cmd_discord_get_member_id_by_username(id)
{
    new memberUsername[32];
    read_argv(1, memberUsername, charsmax(memberUsername));

    new memberId[32];
    new const bool:found = GetGuildMemberIdByUsername(IDENTIFIER, GUILD_ID, memberUsername, memberId, charsmax(memberId));

    if (!found)
    {
        server_print("Member with username %s not found", memberUsername);
        return;
    }

    server_print("Member %s has id %s", memberUsername, memberId);
}

public cmd_discord_get_member_string_by_id(id)
{
    new memberId[32];
    read_argv(1, memberId, charsmax(memberId));

    new nickname[64], username[64], displayName[64];

    new const bool:nicknameFound = GetGuildMemberStringMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_NICKNAME, nickname, charsmax(nickname));
    new const bool:usernameFound = GetGuildMemberStringMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_USERNAME, username, charsmax(username));
    new const bool:displayNameFound = GetGuildMemberStringMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_DISPLAY_NAME, displayName, charsmax(displayName));

    if (!nicknameFound && !usernameFound && !displayNameFound)
    {
        server_print("Member with id %s not found", memberId);
        return;
    }

    server_print("Member %s: nickname=%s, username=%s, display_name=%s", memberId, nickname, username, displayName);
}

public cmd_discord_get_member_int_by_id(id)
{
    new memberId[32];
    read_argv(1, memberId, charsmax(memberId));

    new const joinedAt = GetGuildMemberIntegerMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_JOINED_AT);
    new const commsDisabledUntil = GetGuildMemberIntegerMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_COMMUNICATION_DISABLED_UNTIL);
    new const premiumSince = GetGuildMemberIntegerMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_PREMIUM_SINCE);
    new const isDeaf = GetGuildMemberIntegerMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_IS_DEAF);
    new const isMuted = GetGuildMemberIntegerMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_IS_MUTED);
    new const isPending = GetGuildMemberIntegerMemberById(IDENTIFIER, GUILD_ID, memberId, DAPI_MEMBER_IS_PENDING);

    server_print("Member %s: joined_at=%d, comms_disabled_until=%d, premium_since=%d, is_deaf=%d, is_muted=%d, is_pending=%d", memberId, joinedAt, commsDisabledUntil, premiumSince, isDeaf, isMuted, isPending);
}

public cmd_discord_get_members_count(id)
{
    new const count = GetGuildMembersCount(IDENTIFIER, GUILD_ID);

    server_print("Guild %s has %d cached members", GUILD_ID, count);
}

public cmd_discord_get_member_string_by_index(id)
{
    new count = GetGuildMembersCount(IDENTIFIER, GUILD_ID);

    if (count <= 0)
    {
        server_print("No cached members found for guild %s", GUILD_ID);
        return;
    }

    server_print("Listing string members for %d cached member(s):", count);

    new nickname[64], username[64], displayName[64];

    for (new i = 0; i < count; i++)
    {
        GetGuildMemberStringMemberByIndex(IDENTIFIER, GUILD_ID, i, DAPI_MEMBER_NICKNAME, nickname, charsmax(nickname));
        GetGuildMemberStringMemberByIndex(IDENTIFIER, GUILD_ID, i, DAPI_MEMBER_USERNAME, username, charsmax(username));
        GetGuildMemberStringMemberByIndex(IDENTIFIER, GUILD_ID, i, DAPI_MEMBER_DISPLAY_NAME, displayName, charsmax(displayName));

        server_print("  [%d] nickname=%s, username=%s, display_name=%s", i, nickname, username, displayName);
    }
}

public cmd_discord_get_member_int_by_index(id)
{
    new count = GetGuildMembersCount(IDENTIFIER, GUILD_ID);

    if (count <= 0)
    {
        server_print("No cached members found for guild %s", GUILD_ID);
        return;
    }

    server_print("Listing integer members for %d cached member(s):", count);

    for (new i = 0; i < count; i++)
    {
        new const joinedAt = GetGuildMemberIntegerMemberByIndex(IDENTIFIER, GUILD_ID, i, DAPI_MEMBER_JOINED_AT);
        new const isPending = GetGuildMemberIntegerMemberByIndex(IDENTIFIER, GUILD_ID, i, DAPI_MEMBER_IS_PENDING);

        server_print("  [%d] joined_at=%d, is_pending=%d", i, joinedAt, isPending);
    }
}

public cmd_discord_list_members(id)
{
    new count = GetGuildMembersCount(IDENTIFIER, GUILD_ID);

    server_print("Guild %s has %d cached members total", GUILD_ID, count);
}

public cmd_discord_member_has_role(id)
{
    new memberId[32], roleId[32];
    read_argv(1, memberId, charsmax(memberId));
    read_argv(2, roleId, charsmax(roleId));

    new const bool:hasRole = GuildMemberHasRole(IDENTIFIER, GUILD_ID, memberId, roleId);

    server_print("Member %s %s role %s", memberId, hasRole ? "has" : "does not have", roleId);
}

public cmd_discord_add_member_role(id)
{
    new memberId[32], roleId[32];
    read_argv(1, memberId, charsmax(memberId));
    read_argv(2, roleId, charsmax(roleId));

    new const bool:sent = AddGuildMemberRole(IDENTIFIER, GUILD_ID, memberId, roleId);

    server_print("Add role request %s (member %s, role %s)", sent ? "sent" : "failed to send", memberId, roleId);
}

public cmd_discord_remove_member_role(id)
{
    new memberId[32], roleId[32];
    read_argv(1, memberId, charsmax(memberId));
    read_argv(2, roleId, charsmax(roleId));

    new const bool:sent = RemoveGuildMemberRole(IDENTIFIER, GUILD_ID, memberId, roleId);

    server_print("Remove role request %s (member %s, role %s)", sent ? "sent" : "failed to send", memberId, roleId);
}

public cmd_discord_fetch_member_by_id(id)
{
    new memberId[32];
    read_argv(1, memberId, charsmax(memberId));

    new const bool:sent = FetchGuildUserById(IDENTIFIER, GUILD_ID, memberId);

    server_print("Fetch request %s for member id %s (result via OnGuildMemberFetched)", sent ? "sent" : "failed to send", memberId);
}

public cmd_discord_fetch_member_by_username(id)
{
    new memberUsername[32];
    read_argv(1, memberUsername, charsmax(memberUsername));

    new const bool:sent = FetchGuildUserByUsername(IDENTIFIER, GUILD_ID, memberUsername);

    server_print("Fetch request %s for username %s (result via OnGuildMemberFetched)", sent ? "sent" : "failed to send", memberUsername);
}

public cmd_discord_role_exists_by_id(id)
{
    new roleId[32];
    read_argv(1, roleId, charsmax(roleId));

    server_print("Checking if role with id %s exists", roleId);

    new const bool:exists = GuildRoleExistsById(IDENTIFIER, GUILD_ID, roleId);

    server_print("Role %s", exists ? "exists" : "does not exist");
}

public cmd_discord_role_exists_by_name(id)
{
    new roleName[64];
    read_argv(1, roleName, charsmax(roleName));

    server_print("Checking if role with name %s exists", roleName);

    new const bool:exists = GuildRoleExistsByName(IDENTIFIER, GUILD_ID, roleName);

    server_print("Role %s", exists ? "exists" : "does not exist");
}

public cmd_discord_get_role_id_by_name(id)
{
    new roleName[64];
    read_argv(1, roleName, charsmax(roleName));

    new roleId[32];
    new const bool:found = GetGuildRoleIdByName(IDENTIFIER, GUILD_ID, roleName, roleId, charsmax(roleId));

    if (!found)
    {
        server_print("Role with name %s not found", roleName);
        return;
    }

    server_print("Role %s has id %s", roleName, roleId);
}