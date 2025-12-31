#pragma once
#include <unordered_map>


enum ForwardType
{
	ON_BOT_READY,
	ON_MESSAGE_CREATED,
};

using ForwardsMap = std::unordered_map<ForwardType, int>;

extern const char* g_ForwardsNames[];
extern ForwardsMap g_Forwards;

void RegisterForwards();

template <typename... Args>
void ExecuteForward(ForwardType forward_type, Args... args)
{
	ForwardsMap::iterator it = g_Forwards.find(forward_type);

	if (it == g_Forwards.end() || it->second == -1)
	{
		MF_Log("ERROR: Forward %s has not been registered succesfully!", g_ForwardsNames[static_cast<int>(forward_type)]);
		return;
	}

	MF_ExecuteForward(it->second, args...);
}