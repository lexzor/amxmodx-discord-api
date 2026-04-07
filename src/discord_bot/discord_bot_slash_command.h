#pragma once

#include <dpp/dpp.h>

constexpr int INVALID_SLASH_COMMAND_AMXX_FW_HANDLE = -1;

struct SlashCommand
{
public:
	SlashCommand(const dpp::slashcommand& _slash_command, int _amx_fw_handle)
		: slash_command(_slash_command), amx_fw_handle(_amx_fw_handle) {}

	dpp::slashcommand slash_command;
	int amx_fw_handle;
};