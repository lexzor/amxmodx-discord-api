#if !defined(INCLUDED_IN_MODULE) || defined(__INTELLISENSE__)
#pragma once

#else

import std;

#endif

#ifndef EXPORT
#define EXPORT
#endif

#pragma region monsterevent.h
EXPORT struct MonsterEvent_t
{
	int event{};
	char* options{};
};

EXPORT inline constexpr auto EVENT_SPECIFIC = 0;
EXPORT inline constexpr auto EVENT_SCRIPTED = 1000;
EXPORT inline constexpr auto EVENT_SHARED = 2000;
EXPORT inline constexpr auto EVENT_CLIENT = 5000;

EXPORT inline constexpr auto MONSTER_EVENT_BODYDROP_LIGHT = 2001;
EXPORT inline constexpr auto MONSTER_EVENT_BODYDROP_HEAVY = 2002;
EXPORT inline constexpr auto MONSTER_EVENT_SWISHSOUND = 2010;
#pragma endregion monsterevent.h

#pragma region scriptevent.h

EXPORT enum EScriptEvents
{
	SCRIPT_EVENT_DEAD = 1000,	// character is now dead
	SCRIPT_EVENT_NOINTERRUPT = 1001,	// does not allow interrupt
	SCRIPT_EVENT_CANINTERRUPT = 1002,	// will allow interrupt
	SCRIPT_EVENT_FIREEVENT = 1003,	// event now fires
	SCRIPT_EVENT_SOUND = 1004,	// Play named wave file (on CHAN_BODY)
	SCRIPT_EVENT_SENTENCE = 1005,	// Play named sentence
	SCRIPT_EVENT_INAIR = 1006,	// Leave the character in air at the end of the sequence (don't find the floor)
	SCRIPT_EVENT_ENDANIMATION = 1007,	// Set the animation by name after the sequence completes
	SCRIPT_EVENT_SOUND_VOICE = 1008,	// Play named wave file (on CHAN_VOICE)
	SCRIPT_EVENT_SENTENCE_RND1 = 1009,	// Play sentence group 25% of the time
	SCRIPT_EVENT_NOT_DEAD = 1010,	// Bring back to life (for life/death sequences)
};

#pragma endregion scriptevent.h


#pragma region activity.h
EXPORT enum Activity
{
	ACT_INVALID = -1,

	ACT_RESET = 0,			// Set m_Activity to this invalid value to force a reset to m_IdealActivity
	ACT_IDLE,
	ACT_GUARD,
	ACT_WALK,
	ACT_RUN,
	ACT_FLY,				// Fly (and flap if appropriate)
	ACT_SWIM,
	ACT_HOP,				// vertical jump
	ACT_LEAP,				// long forward jump
	ACT_FALL,
	ACT_LAND,
	ACT_STRAFE_LEFT,
	ACT_STRAFE_RIGHT,
	ACT_ROLL_LEFT,			// tuck and roll, left
	ACT_ROLL_RIGHT,			// tuck and roll, right
	ACT_TURN_LEFT,			// turn quickly left (stationary)
	ACT_TURN_RIGHT,			// turn quickly right (stationary)
	ACT_CROUCH,				// the act of crouching down from a standing position
	ACT_CROUCHIDLE,			// holding body in crouched position (loops)
	ACT_STAND,				// the act of standing from a crouched position
	ACT_USE,
	ACT_SIGNAL1,
	ACT_SIGNAL2,
	ACT_SIGNAL3,
	ACT_TWITCH,
	ACT_COWER,
	ACT_SMALL_FLINCH,
	ACT_BIG_FLINCH,
	ACT_RANGE_ATTACK1,
	ACT_RANGE_ATTACK2,
	ACT_MELEE_ATTACK1,
	ACT_MELEE_ATTACK2,
	ACT_RELOAD,
	ACT_ARM,				// pull out gun, for instance
	ACT_DISARM,				// reholster gun
	ACT_EAT,				// monster chowing on a large food item (loop)
	ACT_DIESIMPLE,
	ACT_DIEBACKWARD,
	ACT_DIEFORWARD,
	ACT_DIEVIOLENT,
	ACT_BARNACLE_HIT,		// barnacle tongue hits a monster
	ACT_BARNACLE_PULL,		// barnacle is lifting the monster ( loop )
	ACT_BARNACLE_CHOMP,		// barnacle latches on to the monster
	ACT_BARNACLE_CHEW,		// barnacle is holding the monster in its mouth ( loop )
	ACT_SLEEP,
	ACT_INSPECT_FLOOR,		// for active idles, look at something on or near the floor
	ACT_INSPECT_WALL,		// for active idles, look at something directly ahead of you ( doesn't HAVE to be a wall or on a wall )
	ACT_IDLE_ANGRY,			// alternate idle animation in which the monster is clearly agitated. (loop)
	ACT_WALK_HURT,			// limp  (loop)
	ACT_RUN_HURT,			// limp  (loop)
	ACT_HOVER,				// Idle while in flight
	ACT_GLIDE,				// Fly (don't flap)
	ACT_FLY_LEFT,			// Turn left in flight
	ACT_FLY_RIGHT,			// Turn right in flight
	ACT_DETECT_SCENT,		// this means the monster smells a scent carried by the air
	ACT_SNIFF,				// this is the act of actually sniffing an item in front of the monster
	ACT_BITE,				// some large monsters can eat small things in one bite. This plays one time, EAT loops.
	ACT_THREAT_DISPLAY,		// without attacking, monster demonstrates that it is angry. (Yell, stick out chest, etc )
	ACT_FEAR_DISPLAY,		// monster just saw something that it is afraid of
	ACT_EXCITED,			// for some reason, monster is excited. Sees something he really likes to eat, or whatever.
	ACT_SPECIAL_ATTACK1,	// very monster specific special attacks.
	ACT_SPECIAL_ATTACK2,
	ACT_COMBAT_IDLE,		// agitated idle.
	ACT_WALK_SCARED,
	ACT_RUN_SCARED,
	ACT_VICTORY_DANCE,		// killed a player, do a victory dance.
	ACT_DIE_HEADSHOT,		// die, hit in head.
	ACT_DIE_CHESTSHOT,		// die, hit in chest
	ACT_DIE_GUTSHOT,		// die, hit in gut
	ACT_DIE_BACKSHOT,		// die, hit in back
	ACT_FLINCH_HEAD,
	ACT_FLINCH_CHEST,
	ACT_FLINCH_STOMACH,
	ACT_FLINCH_LEFTARM,
	ACT_FLINCH_RIGHTARM,
	ACT_FLINCH_LEFTLEG,
	ACT_FLINCH_RIGHTLEG,
	ACT_FLINCH,
	ACT_LARGE_FLINCH,
	ACT_HOLDBOMB,
	ACT_IDLE_FIDGET,
	ACT_IDLE_SCARED,
	ACT_IDLE_SCARED_FIDGET,
	ACT_FOLLOW_IDLE,
	ACT_FOLLOW_IDLE_FIDGET,
	ACT_FOLLOW_IDLE_SCARED,
	ACT_FOLLOW_IDLE_SCARED_FIDGET,
	ACT_CROUCH_IDLE,
	ACT_CROUCH_IDLE_FIDGET,
	ACT_CROUCH_IDLE_SCARED,
	ACT_CROUCH_IDLE_SCARED_FIDGET,
	ACT_CROUCH_WALK,
	ACT_CROUCH_WALK_SCARED,
	ACT_CROUCH_DIE,
	ACT_WALK_BACK,
	ACT_IDLE_SNEAKY,
	ACT_IDLE_SNEAKY_FIDGET,
	ACT_WALK_SNEAKY,
	ACT_WAVE,
	ACT_YES,
	ACT_NO,
};

EXPORT struct activity_map_t
{
	int type{};
	const char* name{};
};

#define _A(a) { a, #a }

EXPORT inline constexpr activity_map_t activity_map[] =
{
	_A(ACT_IDLE),
	_A(ACT_GUARD),
	_A(ACT_WALK),
	_A(ACT_RUN),
	_A(ACT_FLY),
	_A(ACT_SWIM),
	_A(ACT_HOP),
	_A(ACT_LEAP),
	_A(ACT_FALL),
	_A(ACT_LAND),
	_A(ACT_STRAFE_LEFT),
	_A(ACT_STRAFE_RIGHT),
	_A(ACT_ROLL_LEFT),
	_A(ACT_ROLL_RIGHT),
	_A(ACT_TURN_LEFT),
	_A(ACT_TURN_RIGHT),
	_A(ACT_CROUCH),
	_A(ACT_CROUCHIDLE),
	_A(ACT_STAND),
	_A(ACT_USE),
	_A(ACT_SIGNAL1),
	_A(ACT_SIGNAL2),
	_A(ACT_SIGNAL3),
	_A(ACT_TWITCH),
	_A(ACT_COWER),
	_A(ACT_SMALL_FLINCH),
	_A(ACT_BIG_FLINCH),
	_A(ACT_RANGE_ATTACK1),
	_A(ACT_RANGE_ATTACK2),
	_A(ACT_MELEE_ATTACK1),
	_A(ACT_MELEE_ATTACK2),
	_A(ACT_RELOAD),
	_A(ACT_ARM),
	_A(ACT_DISARM),
	_A(ACT_EAT),
	_A(ACT_DIESIMPLE),
	_A(ACT_DIEBACKWARD),
	_A(ACT_DIEFORWARD),
	_A(ACT_DIEVIOLENT),
	_A(ACT_BARNACLE_HIT),
	_A(ACT_BARNACLE_PULL),
	_A(ACT_BARNACLE_CHOMP),
	_A(ACT_BARNACLE_CHEW),
	_A(ACT_SLEEP),
	_A(ACT_INSPECT_FLOOR),
	_A(ACT_INSPECT_WALL),
	_A(ACT_IDLE_ANGRY),
	_A(ACT_WALK_HURT),
	_A(ACT_RUN_HURT),
	_A(ACT_HOVER),
	_A(ACT_GLIDE),
	_A(ACT_FLY_LEFT),
	_A(ACT_FLY_RIGHT),
	_A(ACT_DETECT_SCENT),
	_A(ACT_SNIFF),
	_A(ACT_BITE),
	_A(ACT_THREAT_DISPLAY),
	_A(ACT_FEAR_DISPLAY),
	_A(ACT_EXCITED),
	_A(ACT_SPECIAL_ATTACK1),
	_A(ACT_SPECIAL_ATTACK2),
	_A(ACT_COMBAT_IDLE),
	_A(ACT_WALK_SCARED),
	_A(ACT_RUN_SCARED),
	_A(ACT_VICTORY_DANCE),
	_A(ACT_DIE_HEADSHOT),
	_A(ACT_DIE_CHESTSHOT),
	_A(ACT_DIE_GUTSHOT),
	_A(ACT_DIE_BACKSHOT),
	_A(ACT_FLINCH_HEAD),
	_A(ACT_FLINCH_CHEST),
	_A(ACT_FLINCH_STOMACH),
	_A(ACT_FLINCH_LEFTARM),
	_A(ACT_FLINCH_RIGHTARM),
	_A(ACT_FLINCH_LEFTLEG),
	_A(ACT_FLINCH_RIGHTLEG),
	_A(ACT_FLINCH),
	_A(ACT_LARGE_FLINCH),
	_A(ACT_HOLDBOMB),
	_A(ACT_IDLE_FIDGET),
	_A(ACT_IDLE_SCARED),
	_A(ACT_IDLE_SCARED_FIDGET),
	_A(ACT_FOLLOW_IDLE),
	_A(ACT_FOLLOW_IDLE_FIDGET),
	_A(ACT_FOLLOW_IDLE_SCARED),
	_A(ACT_FOLLOW_IDLE_SCARED_FIDGET),
	_A(ACT_CROUCH_IDLE),
	_A(ACT_CROUCH_IDLE_FIDGET),
	_A(ACT_CROUCH_IDLE_SCARED),
	_A(ACT_CROUCH_IDLE_SCARED_FIDGET),
	_A(ACT_CROUCH_WALK),
	_A(ACT_CROUCH_WALK_SCARED),
	_A(ACT_CROUCH_DIE),
	_A(ACT_WALK_BACK),
	_A(ACT_IDLE_SNEAKY),
	_A(ACT_IDLE_SNEAKY_FIDGET),
	_A(ACT_WALK_SNEAKY),
	_A(ACT_WAVE),
	_A(ACT_YES),
	_A(ACT_NO),
	{ 0, nullptr }
};
#pragma endregion activity.h

// #SHOULD_DO_ON_FREE make a indepent module for PM?
#pragma region pm_materials.h
EXPORT inline constexpr auto CBTEXTURENAMEMAX = 17;			// only load first n chars of name #NO_URGENT original: 13 (HLSDK)

EXPORT inline constexpr auto CHAR_TEX_CONCRETE = 'C';			// texture types
EXPORT inline constexpr auto CHAR_TEX_METAL = 'M';
EXPORT inline constexpr auto CHAR_TEX_DIRT = 'D';
EXPORT inline constexpr auto CHAR_TEX_VENT = 'V';
EXPORT inline constexpr auto CHAR_TEX_GRATE = 'G';
EXPORT inline constexpr auto CHAR_TEX_TILE = 'T';
EXPORT inline constexpr auto CHAR_TEX_SLOSH = 'S';
EXPORT inline constexpr auto CHAR_TEX_WOOD = 'W';
EXPORT inline constexpr auto CHAR_TEX_COMPUTER = 'P';
EXPORT inline constexpr auto CHAR_TEX_GLASS = 'Y';
EXPORT inline constexpr auto CHAR_TEX_FLESH = 'F';

// extended by CZDS
EXPORT inline constexpr auto CHAR_TEX_CARPET = 'E';
EXPORT inline constexpr auto CHAR_TEX_SNOW = 'N';
EXPORT inline constexpr auto CHAR_TEX_GRASS_CZ = 'A';	// CZDS
EXPORT inline constexpr auto CHAR_TEX_GRASS_CS = 'X';	// CSCZ
EXPORT inline constexpr auto CHAR_TEX_GRAVEL = 'R';
#pragma endregion pm_materials.h

#pragma region cdll_dll.h
EXPORT inline constexpr auto MAX_WEAPONS = 32;		// ???

EXPORT inline constexpr auto MAX_WEAPON_SLOTS = 5;	// hud item selection slots
EXPORT inline constexpr auto MAX_ITEM_TYPES = 6;	// hud item selection slots

EXPORT inline constexpr auto MAX_ITEMS = 5;	// hard coded item types

EXPORT inline constexpr auto HIDEHUD_WEAPONS = (1 << 0);
EXPORT inline constexpr auto HIDEHUD_FLASHLIGHT = (1 << 1);
EXPORT inline constexpr auto HIDEHUD_ALL = (1 << 2);
EXPORT inline constexpr auto HIDEHUD_HEALTH = (1 << 3);
EXPORT inline constexpr auto HIDEHUD_TIMER = (1 << 4);
EXPORT inline constexpr auto HIDEHUD_MONEY = (1 << 5);
EXPORT inline constexpr auto HIDEHUD_CROSSHAIR = (1 << 6);

EXPORT inline constexpr auto MAX_AMMO_TYPES = 32;		// ???
EXPORT inline constexpr auto MAX_AMMO_SLOTS = 32;		// not really slots

EXPORT inline constexpr auto HUD_PRINTNOTIFY = 1;
EXPORT inline constexpr auto HUD_PRINTCONSOLE = 2;
EXPORT inline constexpr auto HUD_PRINTTALK = 3;
EXPORT inline constexpr auto HUD_PRINTCENTER = 4;

EXPORT inline constexpr auto DEFAULT_FOV = 90;	// the default field of view

EXPORT inline constexpr auto WEAPON_SUIT = 31;
#pragma endregion cdll_dll.h
