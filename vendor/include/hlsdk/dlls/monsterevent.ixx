/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
export module monsterevent;

export struct MonsterEvent_t
{
	int event{};
	char* options{};
};

export inline constexpr auto EVENT_SPECIFIC = 0;
export inline constexpr auto EVENT_SCRIPTED = 1000;
export inline constexpr auto EVENT_SHARED = 2000;
export inline constexpr auto EVENT_CLIENT = 5000;

export inline constexpr auto MONSTER_EVENT_BODYDROP_LIGHT = 2001;
export inline constexpr auto MONSTER_EVENT_BODYDROP_HEAVY = 2002;
export inline constexpr auto MONSTER_EVENT_SWISHSOUND = 2010;
