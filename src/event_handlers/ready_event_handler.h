#pragma once

#include "dpp/dispatcher.h"

class DiscordBot;

class ReadyEventHandler
{
public:
	ReadyEventHandler(DiscordBot* bot);
	~ReadyEventHandler();

	void RegisterListeners();

	void OnReady();

private:
	DiscordBot* m_Bot;
};