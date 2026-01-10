#if !defined(INCLUDED_IN_MODULE) || defined(__INTELLISENSE__)
#pragma once

#include <experimental/generator>
#include <span>
#include <unordered_map>
#include <vector>

#include "../engine/hlsdk.engine.hpp"

#else

import std;
import <experimental/generator>;	// #MSVC_BUG_GENERATOR

import :engine;

#endif

#ifndef EXPORT
#define EXPORT
#endif

//
// Conversion among the three types of "entity", including identity-conversions.
//
EXPORT template <typename new_t, typename org_t>
[[nodiscard]] inline std::remove_cvref_t<new_t> ent_cast(org_t const& ent) noexcept
{
	if constexpr (std::integral<org_t>)
	{
		if constexpr (std::integral<new_t>)	// entindex -> entindex
			return ent;
		else if constexpr (std::is_same_v<std::remove_cvref_t<new_t>, entvars_t*>)	// entindex -> entvars_t*
			return &g_engfuncs.pfnPEntityOfEntIndex(ent)->v;
		else if constexpr (std::is_same_v<std::remove_cvref_t<new_t>, edict_t*>)	// entindex -> edict_t*
			return g_engfuncs.pfnPEntityOfEntIndex(ent);
		else if constexpr (requires(std::remove_cvref_t<new_t> org) { { org->pev } -> std::convertible_to<entvars_t*>; })	// entindex -> CBase/EHANDLE
			return std::remove_cvref_t<new_t>(g_engfuncs.pfnPEntityOfEntIndex(ent)->pvPrivateData);
		else
			static_assert(false, "Casting to a unsupported type.");
	}
	else if constexpr (std::is_same_v<std::remove_cvref_t<org_t>, entvars_t*>)
	{
		if constexpr (std::integral<new_t>)	// entvars_t* -> entindex
			return g_engfuncs.pfnIndexOfEdict(ent->pContainingEntity);
		else if constexpr (std::is_same_v<std::remove_cvref_t<new_t>, entvars_t*>)	// entvars_t* -> entvars_t*
			return ent;
		else if constexpr (std::is_same_v<std::remove_cvref_t<new_t>, edict_t*>)	// entvars_t* -> edict_t*
			return ent->pContainingEntity;
		else if constexpr (requires(std::remove_cvref_t<new_t> org) { { org->pev } -> std::convertible_to<entvars_t*>; })	// entvars_t* -> CBase/EHANDLE
			return std::remove_cvref_t<new_t>(ent->pContainingEntity->pvPrivateData);
		else
			static_assert(false, "Casting to a unsupported type.");
	}
	else if constexpr (std::is_same_v<std::remove_cvref_t<org_t>, edict_t*>)
	{
		if constexpr (std::integral<new_t>)	// edict_t* -> entindex
			return g_engfuncs.pfnIndexOfEdict(ent);
		else if constexpr (std::is_same_v<std::remove_cvref_t<new_t>, entvars_t*>)	// edict_t* -> entvars_t*
			return &ent->v;
		else if constexpr (std::is_same_v<std::remove_cvref_t<new_t>, edict_t*>)	// edict_t* -> edict_t*
			return ent;
		else if constexpr (requires(std::remove_cvref_t<new_t> org) { { org->pev } -> std::convertible_to<entvars_t*>; })	// edict_t* -> CBase/EHANDLE
			return std::remove_cvref_t<new_t>(ent->pvPrivateData);
		else
			static_assert(false, "Casting to a unsupported type.");
	}

	// This is for CBaseEntity, but we are not going to restricting class name nor import CBase.
	else if constexpr (requires { { ent->pev } -> std::convertible_to<entvars_t*>; })
	{
		return ent_cast<new_t>(ent->pev);
	}
	else
		static_assert(false, "Casting from a unsupported type.");
}

EXPORT inline auto ENTOFFSET(edict_t* pEdict) noexcept { return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict); }	// eoffset is different from entindex!!
EXPORT inline auto OFFSETENT(int iEdictNum) noexcept { return (*g_engfuncs.pfnPEntityOfEntOffset)(iEdictNum); }

// Testing the three types of "entity" for nullity
//#define eoNullEntity 0
//inline BOOL FNullEnt(EOFFSET eoffset) { return eoffset == 0; }
//inline BOOL FNullEnt(const edict_t *pent) { return pent == NULL || FNullEnt(OFFSET(pent)); }
//inline BOOL FNullEnt(entvars_t *pev) { return pev == NULL || FNullEnt(OFFSET(pev)); }
EXPORT enum struct EValidity
{
	Not = 0,
	Unmanaged = 1,
	Full = 2,
};

EXPORT inline EValidity pev_valid(entvars_t* pev) noexcept
{
	if (pev == nullptr || ent_cast<int>(pev->pContainingEntity) <= 0)
		return EValidity::Not;

	if (auto const pEdict = ent_cast<edict_t*>(pev); pEdict && pEdict->pvPrivateData != nullptr)
		return EValidity::Full;

	return EValidity::Unmanaged;
}

EXPORT __forceinline EValidity pev_valid(edict_t* pEdict) noexcept { return pEdict ? pev_valid(&pEdict->v) : EValidity::Not; }

// Use this instead of ALLOC_STRING on constant strings
EXPORT inline const char* STRING(string_t iOffset) noexcept { return gpGlobals->pStringBase + iOffset; }
EXPORT template <size_t N> inline string_t MAKE_STRING(const char(&rgsz)[N]) noexcept { return string_t{ (&rgsz[0]) - gpGlobals->pStringBase }; }
EXPORT inline string_t MAKE_STRING_UNSAFE(const char* psz) noexcept { return string_t{ psz - gpGlobals->pStringBase }; }

//
// Search Entity
//

// LUNA: Try Query lib from CSDK instead.
EXPORT inline std::experimental::generator<edict_t*> FIND_ENTITY_BY_CLASSNAME(const char* pszName) noexcept
{
	for (auto pEdict = g_engfuncs.pfnFindEntityByString(nullptr, "classname", pszName);
		pev_valid(pEdict) == EValidity::Full;
		pEdict = g_engfuncs.pfnFindEntityByString(pEdict, "classname", pszName))
	{
		co_yield pEdict;
	}

	co_return;
}

//inline edict_t *FIND_ENTITY_BY_TARGETNAME(edict_t *entStart, const char *pszName)
//{
//	return FIND_ENTITY_BY_STRING(entStart, "targetname", pszName);
//}

// for doing a reverse lookup. Say you have a door, and want to find its button.
//inline edict_t *FIND_ENTITY_BY_TARGET(edict_t *entStart, const char *pszName)
//{
//	return FIND_ENTITY_BY_STRING(entStart, "target", pszName);
//}

// LUNA: Try Query lib from CSDK instead.
EXPORT std::experimental::generator<edict_t*> FIND_ENTITY_IN_SPHERE(const Vector vecOrigin, float const flRadius) noexcept
{
	for (auto pEdict = g_engfuncs.pfnFindEntityInSphere(nullptr, vecOrigin, flRadius);
		pev_valid(pEdict) == EValidity::Full;
		pEdict = g_engfuncs.pfnFindEntityInSphere(pEdict, vecOrigin, flRadius))
	{
		co_yield pEdict;
	}

	co_return;
}

// Keeps clutter down a bit, when writing key-value pairs
EXPORT inline void WRITEKEY_INT(void* pf, const char* szKeyName, int iKeyValue) noexcept
{
	g_engfuncs.pfnEngineFprintf(pf, "\"%s\" \"%d\"\n", szKeyName, iKeyValue);
}

EXPORT inline void WRITEKEY_FLOAT(void* pf, const char* szKeyName, float flKeyValue) noexcept
{
	g_engfuncs.pfnEngineFprintf(pf, "\"%s\" \"%f\"\n", szKeyName, flKeyValue);
}

EXPORT inline void WRITEKEY_STRING(void* pf, const char* szKeyName, const char* szKeyValue) noexcept
{
	g_engfuncs.pfnEngineFprintf(pf, "\"%s\" \"%s\"\n", szKeyName, szKeyValue);
}

EXPORT inline void WRITEKEY_VECTOR(void* pf, const char* szKeyName, Vector const& vKeyValue) noexcept
{
	g_engfuncs.pfnEngineFprintf(pf, "\"%s\" \"%f %f %f\"\n", szKeyName, vKeyValue.x, vKeyValue.y, vKeyValue.z);
}

// Keeps clutter down a bit, when using a float as a bit-vector
//#define SetBits(flBitVector, bits)              ((flBitVector) = (int)(flBitVector) | (bits))
//#define ClearBits(flBitVector, bits)    ((flBitVector) = (int)(flBitVector) & ~(bits))
//#define FBitSet(flBitVector, bit)               ((int)(flBitVector) & (bit))

// Makes these more explicit, and easier to find
//#define FILE_GLOBAL static
//#define DLL_GLOBAL

// Until we figure out why "const" gives the compiler problems, we'll just have to use
// this bogus "empty" define to mark things as constant.
//#define CONSTANT

// More explicit than "int"
//typedef int EOFFSET;

// In case it's not alread defined
//typedef int BOOL;

// In case this ever changes // LUNA: try std::numbers::pi
//#define M_PI                    3.14159265358979323846

// Keeps clutter down a bit, when declaring external entity/global method prototypes
//#define DECLARE_GLOBAL_METHOD(MethodName)  extern void DLLEXPORT MethodName( void )
//#define GLOBAL_METHOD(funcname)                                 void DLLEXPORT funcname(void)

// This is the glue that hooks .MAP entity class names to our CPP classes
// The _declspec forces them to be exported by name so we can do a lookup with GetProcAddress()
// The function is used to intialize / allocate the object for the entity
//#ifdef _WIN32
//#define LINK_ENTITY_TO_CLASS(mapClassName,DLLClassName) \
//        extern "C" __declspec( dllexport ) void mapClassName( entvars_t *pev ); \
//        void mapClassName( entvars_t *pev ) { GetClassPtr( (DLLClassName *)pev ); }
//#else
//#define LINK_ENTITY_TO_CLASS(mapClassName,DLLClassName) extern "C" void mapClassName( entvars_t *pev ); void mapClassName( entvars_t *pev ) { GetClassPtr( (DLLClassName *)pev ); }
//#endif

//inline void MESSAGE_BEGIN(int msg_dest, int msg_type, const float *pOrigin, entvars_t *ent) {
//	(*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ENT(ent));
//}

// Testing strings for nullity
//#define iStringNull 0
EXPORT inline qboolean FStringNull(string_t iString) noexcept { return iString == string_t{ 0 }; }

EXPORT inline constexpr size_t cchMapNameMost = 32;

// Dot products for view cone checking
EXPORT inline constexpr float VIEW_FIELD_FULL = -1.f; // +-180 degrees
EXPORT inline constexpr float VIEW_FIELD_WIDE = -0.7f; // +-135 degrees 0.1 // +-85 degrees, used for full FOV checks 
EXPORT inline constexpr float VIEW_FIELD_NARROW = 0.7f; // +-45 degrees, more narrow check used to set up ranged attacks
EXPORT inline constexpr float VIEW_FIELD_ULTRA_NARROW = 0.9f; // +-25 degrees, more narrow check used to set up ranged attacks

// All monsters need this data
EXPORT inline constexpr uint8_t DONT_BLEED = -1;
EXPORT inline constexpr uint8_t BLOOD_COLOR_RED = 247;
EXPORT inline constexpr uint8_t BLOOD_COLOR_YELLOW = 195;
EXPORT inline constexpr uint8_t BLOOD_COLOR_GREEN = BLOOD_COLOR_YELLOW;	// LUNA: WTF??

EXPORT enum MONSTERSTATE : uint32_t
{
	MONSTERSTATE_NONE = 0,
	MONSTERSTATE_IDLE,
	MONSTERSTATE_COMBAT,
	MONSTERSTATE_ALERT,
	MONSTERSTATE_HUNT,
	MONSTERSTATE_PRONE,
	MONSTERSTATE_SCRIPT,
	MONSTERSTATE_PLAYDEAD,
	MONSTERSTATE_DEAD
};

// Things that toggle (buttons/triggers/doors) need this
EXPORT enum TOGGLE_STATE : uint32_t
{
	TS_AT_TOP,
	TS_AT_BOTTOM,
	TS_GOING_UP,
	TS_GOING_DOWN
};

// Misc useful
EXPORT inline bool FStrEq(const char* psz1, const char* psz2) noexcept { return strcmp(psz1, psz2) == 0; }
EXPORT inline bool FClassnameIs(entvars_t* pent, const char* szClassname) noexcept { return strcmp(STRING(pent->classname), szClassname) == 0; }

// Misc. Prototypes
//extern void             UTIL_SetSize(entvars_t *pev, const Vector &vecMin, const Vector &vecMax);
//extern float            UTIL_VecToYaw(const Vector &vec);
//extern Vector           UTIL_VecToAngles(const Vector &vec);
//extern float            UTIL_AngleMod(float a);
//extern float            UTIL_AngleDiff(float destAngle, float srcAngle);

//#define UTIL_EntitiesInPVS(pent)                        (*g_engfuncs.pfnEntitiesInPVS)(pent)
//extern void                     UTIL_MakeVectors(const Vector &vecAngles);

//inline void UTIL_MakeVectorsPrivate(Vector &vecAngles, float *p_vForward, float *p_vRight, float *p_vUp)
//{
//	g_engfuncs.pfnAngleVectors(vecAngles, p_vForward, p_vRight, p_vUp);
//}

//extern void                     UTIL_MakeAimVectors(const Vector &vecAngles); // like MakeVectors, but assumes pitch isn't inverted
//extern void                     UTIL_MakeInvVectors(const Vector &vec, globalvars_t *pgv);

//extern void                     UTIL_SetOrigin(entvars_t *pev, const Vector &vecOrigin);
//extern void                     UTIL_EmitAmbientSound(edict_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch);
//extern void                     UTIL_ParticleEffect(const Vector &vecOrigin, const Vector &vecDirection, ULONG ulColor, ULONG ulCount);
//extern void                     UTIL_ScreenShake(const Vector &center, float amplitude, float frequency, float duration, float radius);
//extern void                     UTIL_ScreenShakeAll(const Vector &center, float amplitude, float frequency, float duration);
//extern void                     UTIL_ShowMessageAll(const char *pString);
//extern void                     UTIL_ScreenFadeAll(const Vector &color, float fadeTime, float holdTime, int alpha, int flags);

// LUNA: trace line flags moved to hlsdk.engine.hpp

//extern void                     UTIL_TraceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr);
//extern void                     UTIL_TraceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr);


//extern void             UTIL_TraceHull(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr);
//extern TraceResult      UTIL_GetGlobalTrace(void);
//extern void             UTIL_TraceModel(const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr);
//extern Vector           UTIL_GetAimVector(edict_t *pent, float flSpeed);
//extern int              UTIL_PointContents(const Vector &vec);

//extern void             UTIL_BloodStream(const Vector &origin, const Vector &direction, int color, int amount);
//extern void             UTIL_BloodDrips(const Vector &origin, const Vector &direction, int color, int amount);
//extern Vector           UTIL_RandomBloodVector(void);
//extern BOOL             UTIL_ShouldShowBlood(int bloodColor);
//extern void             UTIL_BloodDecalTrace(TraceResult *pTrace, int bloodColor);
//extern void             UTIL_DecalTrace(TraceResult *pTrace, int decalNumber);
//extern void             UTIL_PlayerDecalTrace(TraceResult *pTrace, int playernum, int decalNumber, BOOL bIsCustom);
//extern void             UTIL_GunshotDecalTrace(TraceResult *pTrace, int decalNumber);
//extern void             UTIL_Sparks(const Vector &position);
//extern void             UTIL_Ricochet(const Vector &position, float scale);
//extern void             UTIL_StringToVector(float *pVector, const char *pString);
//extern void             UTIL_StringToIntArray(int *pVector, int count, const char *pString);
//extern Vector           UTIL_ClampVectorToBox(const Vector &input, const Vector &clampSize);
//extern float            UTIL_Approach(float target, float value, float speed);
//extern float            UTIL_ApproachAngle(float target, float value, float speed);
//extern float            UTIL_AngleDistance(float next, float cur);

//extern char*            UTIL_VarArgs(char *format, ...);
//extern BOOL             UTIL_IsValidEntity(edict_t *pent);
//extern BOOL             UTIL_TeamsMatch(const char *pTeamName1, const char *pTeamName2);

// Use for ease-in, ease-out style interpolation (accel/decel)
//extern float            UTIL_SplineFraction(float value, float scale);

// Search for water transition along a vertical line
//extern float            UTIL_WaterLevel(const Vector &position, float minz, float maxz);
//extern void             UTIL_Bubbles(Vector mins, Vector maxs, int count);
//extern void             UTIL_BubbleTrail(Vector from, Vector to, int count);

// allows precacheing of other entities
//extern void             UTIL_PrecacheOther(const char *szClassname);

// prints a message to each client
//extern void             UTIL_ClientPrintAll(int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL);
//inline void             UTIL_CenterPrintAll(const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL)
//{
//	UTIL_ClientPrintAll(HUD_PRINTCENTER, msg_name, param1, param2, param3, param4);
//}

//class CBasePlayerItem;
//class CBasePlayer;
//extern BOOL UTIL_GetNextBestWeapon(CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon);

// prints messages through the HUD
//extern void ClientPrint(entvars_t *client, int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL);

EXPORT struct hudtextparms_t
{
	float		x;
	float		y;
	int			effect;
	uint8_t		r1, g1, b1, a1;
	uint8_t		r2, g2, b2, a2;
	float		fadeinTime;
	float		fadeoutTime;
	float		holdTime;
	float		fxTime;
	int			channel;
};

// prints as transparent 'title' to the HUD
//extern void                     UTIL_HudMessageAll(const hudtextparms_t &textparms, const char *pMessage);

// for handy use with ClientPrint params
//extern char *UTIL_dtos1(int d);
//extern char *UTIL_dtos2(int d);
//extern char *UTIL_dtos3(int d);
//extern char *UTIL_dtos4(int d);

// Writes message to console with timestamp and FragLog header.
//extern void                     UTIL_LogPrintf(char *fmt, ...);

// Sorta like FInViewCone, but for nonmonsters. 
//extern float UTIL_DotPoints(const Vector &vecSrc, const Vector &vecCheck, const Vector &vecDir);

//extern void UTIL_StripToken(const char *pKey, char *pDest);// for redundant keynames

// Misc functions
//extern void SetMovedir(entvars_t *pev);
//extern Vector VecBModelOrigin(entvars_t *pevBModel);
//extern int BuildChangeList(LEVELLIST *pLevelList, int maxList);

//
// How did I ever live without ASSERT?
//
//#ifdef  DEBUG
//void DBG_AssertFunction(BOOL fExpr, const char *szExpr, const char *szFile, int szLine, const char *szMessage);
//#define ASSERT(f)               DBG_AssertFunction(f, #f, __FILE__, __LINE__, NULL)
//#define ASSERTSZ(f, sz) DBG_AssertFunction(f, #f, __FILE__, __LINE__, sz)
//#else   // !DEBUG
//#define ASSERT(f)
//#define ASSERTSZ(f, sz)
//#endif  // !DEBUG

EXPORT inline constexpr Vector g_vecZero = Vector::Zero();

//
// Constants that were used only by QC (maybe not used at all now)
//
// Un-comment only as needed
//
EXPORT inline constexpr auto LANGUAGE_ENGLISH = 0;
EXPORT inline constexpr auto LANGUAGE_GERMAN = 1;
EXPORT inline constexpr auto LANGUAGE_FRENCH = 2;
EXPORT inline constexpr auto LANGUAGE_BRITISH = 3;

//extern DLL_GLOBAL int                   g_Language;

EXPORT inline constexpr auto AMBIENT_SOUND_STATIC = 0;	// medium radius attenuation
EXPORT inline constexpr auto AMBIENT_SOUND_EVERYWHERE = 1;
EXPORT inline constexpr auto AMBIENT_SOUND_SMALLRADIUS = 2;
EXPORT inline constexpr auto AMBIENT_SOUND_MEDIUMRADIUS = 4;
EXPORT inline constexpr auto AMBIENT_SOUND_LARGERADIUS = 8;
EXPORT inline constexpr auto AMBIENT_SOUND_START_SILENT = 16;
EXPORT inline constexpr auto AMBIENT_SOUND_NOT_LOOPING = 32;

EXPORT inline constexpr auto SPEAKER_START_SILENT = 1;	// wait for trigger 'on' to start announcements

// LUNA: sound flags moved to hlsdk.engine.hpp

EXPORT inline constexpr auto LFO_SQUARE = 1;
EXPORT inline constexpr auto LFO_TRIANGLE = 2;
EXPORT inline constexpr auto LFO_RANDOM = 3;

// func_rotating
EXPORT inline constexpr auto SF_BRUSH_ROTATE_Y_AXIS = 0;
EXPORT inline constexpr auto SF_BRUSH_ROTATE_INSTANT = 1;
EXPORT inline constexpr auto SF_BRUSH_ROTATE_BACKWARDS = 2;
EXPORT inline constexpr auto SF_BRUSH_ROTATE_Z_AXIS = 4;
EXPORT inline constexpr auto SF_BRUSH_ROTATE_X_AXIS = 8;
EXPORT inline constexpr auto SF_PENDULUM_AUTO_RETURN = 16;
EXPORT inline constexpr auto SF_PENDULUM_PASSABLE = 32;


EXPORT inline constexpr auto SF_BRUSH_ROTATE_SMALLRADIUS = 128;
EXPORT inline constexpr auto SF_BRUSH_ROTATE_MEDIUMRADIUS = 256;
EXPORT inline constexpr auto SF_BRUSH_ROTATE_LARGERADIUS = 512;

EXPORT inline constexpr auto PUSH_BLOCK_ONLY_X = 1;
EXPORT inline constexpr auto PUSH_BLOCK_ONLY_Y = 2;

EXPORT inline constexpr auto VEC_HULL_MIN = Vector(-16, -16, -36);
EXPORT inline constexpr auto VEC_HULL_MAX = Vector(16, 16, 36);
EXPORT inline constexpr auto VEC_HUMAN_HULL_MIN = Vector(-16, -16, 0);
EXPORT inline constexpr auto VEC_HUMAN_HULL_MAX = Vector(16, 16, 72);
EXPORT inline constexpr auto VEC_HUMAN_HULL_DUCK = Vector(16, 16, 36);

EXPORT inline constexpr auto VEC_VIEW = Vector(0, 0, 28);

EXPORT inline constexpr auto VEC_DUCK_HULL_MIN = Vector(-16, -16, -18);
EXPORT inline constexpr auto VEC_DUCK_HULL_MAX = Vector(16, 16, 18);
EXPORT inline constexpr auto VEC_DUCK_VIEW = Vector(0, 0, 12);

// LUNA: SVC_ moved to hlsdk.engine.hpp

// prxoy director stuff
EXPORT inline constexpr auto DRC_EVENT = 3;       // informs the dircetor about ann important game event

EXPORT inline constexpr auto DRC_FLAG_PRIO_MASK = 0x0F;    //      priorities between 0 and 15 (15 most important)
EXPORT inline constexpr auto DRC_FLAG_DRAMATIC = (1 << 5);

// triggers
EXPORT inline constexpr auto SF_TRIGGER_ALLOWMONSTERS = 1;// monsters allowed to fire this trigger
EXPORT inline constexpr auto SF_TRIGGER_NOCLIENTS = 2;// players not allowed to fire this trigger
EXPORT inline constexpr auto SF_TRIGGER_PUSHABLES = 4;// only pushables can fire this trigger

// func breakable
EXPORT inline constexpr auto SF_BREAK_TRIGGER_ONLY = 1;// may only be broken by trigger
EXPORT inline constexpr auto SF_BREAK_TOUCH = 2;// can be 'crashed through' by running player (plate glass)
EXPORT inline constexpr auto SF_BREAK_PRESSURE = 4;// can be broken by a player standing on it
EXPORT inline constexpr auto SF_BREAK_CROWBAR = 256;// instant break if hit with crowbar

// func_pushable (it's also func_breakable, so don't collide with those flags)
EXPORT inline constexpr auto SF_PUSH_BREAKABLE = 128;

EXPORT inline constexpr auto SF_LIGHT_START_OFF = 1;

EXPORT inline constexpr auto SPAWNFLAG_NOMESSAGE = 1;
EXPORT inline constexpr auto SPAWNFLAG_NOTOUCH = 2;	// LUNA: #NO_URGENT original value = 1
EXPORT inline constexpr auto SPAWNFLAG_DROIDONLY = 4;

EXPORT inline constexpr auto SPAWNFLAG_USEONLY = 1;	// can't be touched, must be used (buttons)

EXPORT inline constexpr auto TELE_PLAYER_ONLY = 1;
EXPORT inline constexpr auto TELE_SILENT = 2;

EXPORT inline constexpr auto SF_TRIG_PUSH_ONCE = 1;


// Sound Utilities

// sentence groups
EXPORT inline constexpr size_t CBSENTENCENAME_MAX = 16;
EXPORT inline constexpr size_t CVOXFILESENTENCEMAX = 1536;            // max number of sentences in game. NOTE: this must match
// CVOXFILESENTENCEMAX in engine\sound.h!!!

//extern char gszallsentencenames[CVOXFILESENTENCEMAX][CBSENTENCENAME_MAX];
//extern int gcallsentences;
//
//int USENTENCEG_Pick(int isentenceg, char *szfound);
//int USENTENCEG_PickSequential(int isentenceg, char *szfound, int ipick, int freset);
//void USENTENCEG_InitLRU(unsigned char *plru, int count);
//
//void SENTENCEG_Init();
//void SENTENCEG_Stop(edict_t *entity, int isentenceg, int ipick);
//int SENTENCEG_PlayRndI(edict_t *entity, int isentenceg, float volume, float attenuation, int flags, int pitch);
//int SENTENCEG_PlayRndSz(edict_t *entity, const char *szrootname, float volume, float attenuation, int flags, int pitch);
//int SENTENCEG_PlaySequentialSz(edict_t *entity, const char *szrootname, float volume, float attenuation, int flags, int pitch, int ipick, int freset);
//int SENTENCEG_GetIndex(const char *szrootname);
//int SENTENCEG_Lookup(const char *sample, char *sentencenum);
//
//void TEXTURETYPE_Init();
//char TEXTURETYPE_Find(char *name);
//float TEXTURETYPE_PlaySound(TraceResult *ptr, Vector vecSrc, Vector vecEnd, int iBulletType);

// NOTE: use EMIT_SOUND_DYN to set the pitch of a sound. Pitch of 100
// is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
// down to 1 is a lower pitch.   150 to 70 is the realistic range.
// EMIT_SOUND_DYN with pitch != 100 should be used sparingly, as it's not quite as
// fast as EMIT_SOUND (the pitchshift mixer is not native coded).

//void EMIT_SOUND_DYN(edict_t *entity, int channel, const char *sample, float volume, float attenuation,
//	int flags, int pitch);
//
//
//inline void EMIT_SOUND(edict_t *entity, int channel, const char *sample, float volume, float attenuation)
//{
//	EMIT_SOUND_DYN(entity, channel, sample, volume, attenuation, 0, PITCH_NORM);
//}
//
//inline void STOP_SOUND(edict_t *entity, int channel, const char *sample)
//{
//	EMIT_SOUND_DYN(entity, channel, sample, 0, 0, SND_STOP, PITCH_NORM);
//}
//
//void EMIT_SOUND_SUIT(edict_t *entity, const char *sample);
//void EMIT_GROUPID_SUIT(edict_t *entity, int isentenceg);
//void EMIT_GROUPNAME_SUIT(edict_t *entity, const char *groupname);
//
//#define PRECACHE_SOUND_ARRAY( a ) \
//        { for (int i = 0; i < ARRAYSIZE( a ); i++ ) PRECACHE_SOUND((char *) a [i]); }
//
//#define EMIT_SOUND_ARRAY_DYN( chan, array ) \
//        EMIT_SOUND_DYN ( ENT(pev), chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], 1.0, ATTN_NORM, 0, RANDOM_LONG(95,105) ); 
//
//#define RANDOM_SOUND_ARRAY( array ) (array) [ RANDOM_LONG(0,ARRAYSIZE( (array) )-1) ]
//
//#define PLAYBACK_EVENT( flags, who, index ) PLAYBACK_EVENT_FULL( flags, who, index, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
//#define PLAYBACK_EVENT_DELAY( flags, who, index, delay ) PLAYBACK_EVENT_FULL( flags, who, index, delay, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
//
//#define GROUP_OP_AND    0
//#define GROUP_OP_NAND   1
//
//extern int g_groupmask;
//extern int g_groupop;
//
//class UTIL_GroupTrace
//{
//public:
//	UTIL_GroupTrace(int groupmask, int op);
//	~UTIL_GroupTrace(void);
//
//private:
//	int m_oldgroupmask, m_oldgroupop;
//};
//
//void UTIL_SetGroupTrace(int groupmask, int op);
//void UTIL_UnsetGroupTrace(void);
//
//int UTIL_SharedRandomLong(unsigned int seed, int low, int high);
//float UTIL_SharedRandomFloat(unsigned int seed, float low, float high);
//
//float UTIL_WeaponTimeBase(void);
//
// LUNA's extension
//

/*
* Part I: Trace Helper
*/

// for pfnSetGroupMask
// The engine will skip entity for physent[], before executing PM_Move.
// Some hack can be done if we do PlayerPreThink(), groupinfo, for example.
// Adjust Groupinfo op function to achieve these.
// As for non-players, use ShouldCollide() instead.
// LUNA 1/29/2024: the first parameter of pfnSetGroupMask was for global mask.
// Only involved in functions invoking SV_LinkContents. pfnPointContents, for one example.
// In the case of pfnPointContents, the global mask work as if it were the mask of ignoreEntity->v.groupinfo in pfnTraceLine.
// In the cases of when operator checks out, the point contented in such masked entity will not be checked.
EXPORT enum ESetGroupMaskOp
{
	/*
	* if ( mode == GROUP_OP_AND && (check->pev->groupinfo & player->pev->groupinfo) == 0 )
		continue;
	*/
	GROUP_OP_AND = 0,

	/*
	* if ( mode == GROUP_OP_NAND && (check->pev->groupinfo & player->pev->groupinfo) != 0 )
		continue;
	*/
	GROUP_OP_NAND
};

EXPORT inline constexpr uint32_t GROUPMASK_GROUP_TRACE = (1 << 8);

// Allows you to set a bunch of entities to skip.
EXPORT inline void UTIL_TraceLine(Vector const &vecSrc, Vector const &vecEnd, edict_t *pPlayer, std::span<edict_t *const> rgpEntsToSkip, TraceResult *pTr) noexcept
{
	g_engfuncs.pfnSetGroupMask(0, GROUP_OP_NAND);

	auto const iPlayerLastGroupInfo = pPlayer->v.groupinfo;
	pPlayer->v.groupinfo |= GROUPMASK_GROUP_TRACE;

	std::vector<int> rgiEdictsLastGroupInfo(rgpEntsToSkip.size());
	for (size_t i = 0; i < rgpEntsToSkip.size(); ++i)
	{
		rgiEdictsLastGroupInfo[i] = rgpEntsToSkip[i]->v.groupinfo;
		rgpEntsToSkip[i]->v.groupinfo |= GROUPMASK_GROUP_TRACE;
	}

	// Player goes into param ignoreEntity, therefore ignored by default.
	// After that, the engine will also use ignoreEntity->v.groupinfo to compare with other entities it encountered.
	g_engfuncs.pfnTraceLine(vecSrc, vecEnd, dont_ignore_monsters | dont_ignore_glass, pPlayer, pTr);

	pPlayer->v.groupinfo = iPlayerLastGroupInfo;
	for (size_t i = 0; i < rgpEntsToSkip.size(); ++i)
		rgpEntsToSkip[i]->v.groupinfo = rgiEdictsLastGroupInfo[i];

	g_engfuncs.pfnSetGroupMask(0, 0);
}

EXPORT inline void UTIL_TraceHull(Vector const &vecSrc, Vector const &vecEnd, HULL_TYPES iHullIndex, edict_t *pPlayer, std::span<edict_t *const> rgpEntsToSkip, TraceResult *pTr) noexcept
{
	g_engfuncs.pfnSetGroupMask(0, GROUP_OP_NAND);

	auto const iPlayerLastGroupInfo = pPlayer->v.groupinfo;
	pPlayer->v.groupinfo |= GROUPMASK_GROUP_TRACE;

	std::vector<int> rgiEdictsLastGroupInfo(rgpEntsToSkip.size());
	for (size_t i = 0; i < rgpEntsToSkip.size(); ++i)
	{
		rgiEdictsLastGroupInfo[i] = rgpEntsToSkip[i]->v.groupinfo;
		rgpEntsToSkip[i]->v.groupinfo |= GROUPMASK_GROUP_TRACE;
	}

	// Player goes into param ignoreEntity, therefore ignored by default.
	// After that, the engine will also use ignoreEntity->v.groupinfo to compare with other entities it encountered.
	g_engfuncs.pfnTraceHull(vecSrc, vecEnd, dont_ignore_monsters | dont_ignore_glass, iHullIndex, pPlayer, pTr);

	pPlayer->v.groupinfo = iPlayerLastGroupInfo;
	for (size_t i = 0; i < rgpEntsToSkip.size(); ++i)
		rgpEntsToSkip[i]->v.groupinfo = rgiEdictsLastGroupInfo[i];

	g_engfuncs.pfnSetGroupMask(0, 0);
}

/*
* Part II: Message Helper
*/

// Decal Helper
EXPORT void UTIL_Decal(edict_t *pent, Vector const& vecOrigin, short iDecalTextureIndex) noexcept
{
	auto const iEntityIndex = ent_cast<short>(pent);

	g_engfuncs.pfnMessageBegin(MSG_BROADCAST, SVC_TEMPENTITY, nullptr, nullptr);

	if (iEntityIndex > 0)
	{
		if (iDecalTextureIndex >= 256)
			g_engfuncs.pfnWriteByte(TE_DECALHIGH);
		else
			g_engfuncs.pfnWriteByte(TE_DECAL);
	}
	else
	{
		if (iDecalTextureIndex >= 256)
			g_engfuncs.pfnWriteByte(TE_WORLDDECALHIGH);
		else
			g_engfuncs.pfnWriteByte(TE_WORLDDECAL);
	}

	g_engfuncs.pfnWriteCoord(vecOrigin.x);
	g_engfuncs.pfnWriteCoord(vecOrigin.y);
	g_engfuncs.pfnWriteCoord(vecOrigin.z);

	g_engfuncs.pfnWriteByte(iDecalTextureIndex >= 256 ? (iDecalTextureIndex - 256) : iDecalTextureIndex);

	if (iEntityIndex > 0)
		g_engfuncs.pfnWriteShort(iEntityIndex);

	g_engfuncs.pfnMessageEnd();
}

EXPORT void UTIL_BreakModel(const Vector &vecOrigin, const Vector &vecScale, const Vector &vecVelocity, float flRandSpeedVar, short iModel, uint8_t iCount, float flLife, BREAK_MODEL_TYPE bitsFlags) noexcept
{
	g_engfuncs.pfnMessageBegin(MSG_BROADCAST, SVC_TEMPENTITY, nullptr, nullptr);
	g_engfuncs.pfnWriteByte(TE_BREAKMODEL);

	g_engfuncs.pfnWriteCoord(vecOrigin.x);
	g_engfuncs.pfnWriteCoord(vecOrigin.y);
	g_engfuncs.pfnWriteCoord(vecOrigin.z);

	g_engfuncs.pfnWriteCoord(vecScale.x);
	g_engfuncs.pfnWriteCoord(vecScale.y);
	g_engfuncs.pfnWriteCoord(vecScale.z);

	g_engfuncs.pfnWriteCoord(vecVelocity.x);
	g_engfuncs.pfnWriteCoord(vecVelocity.y);
	g_engfuncs.pfnWriteCoord(vecVelocity.z);

	g_engfuncs.pfnWriteByte(std::lroundf(flRandSpeedVar * 10.f));
	g_engfuncs.pfnWriteShort(iModel);
	g_engfuncs.pfnWriteByte(iCount);
	g_engfuncs.pfnWriteByte(std::lroundf(flLife * 10.f));
	g_engfuncs.pfnWriteByte(bitsFlags);
	g_engfuncs.pfnMessageEnd();
};

EXPORT void UTIL_ExplodeModel(const Vector &vecOrigin, float flSpeed, short iModel, uint8_t iCount, float flLife) noexcept
{
	g_engfuncs.pfnMessageBegin(MSG_BROADCAST, SVC_TEMPENTITY, nullptr, nullptr);
	g_engfuncs.pfnWriteByte(TE_EXPLODEMODEL);

	g_engfuncs.pfnWriteCoord(vecOrigin.x);
	g_engfuncs.pfnWriteCoord(vecOrigin.y);
	g_engfuncs.pfnWriteCoord(vecOrigin.z);

	g_engfuncs.pfnWriteCoord(flSpeed);

	g_engfuncs.pfnWriteShort(iModel);
	g_engfuncs.pfnWriteShort(iCount);
	g_engfuncs.pfnWriteByte(std::lroundf(flLife * 10.f));

	g_engfuncs.pfnMessageEnd();
};

EXPORT void UTIL_Shockwave(Vector const &vecOrigin, float flRadius, short iSprite, uint8_t iStartingFrame, float flFrameRate, float flLife, float flLineWidth, float flNoiseAmp, color24 Color, uint8_t iBrightness, float flScrollSpeed) noexcept
{
	g_engfuncs.pfnMessageBegin(MSG_PVS, SVC_TEMPENTITY, vecOrigin, nullptr);
	g_engfuncs.pfnWriteByte(TE_BEAMCYLINDER);
	g_engfuncs.pfnWriteCoord(vecOrigin.x);
	g_engfuncs.pfnWriteCoord(vecOrigin.y);
	g_engfuncs.pfnWriteCoord(vecOrigin.z);

	g_engfuncs.pfnWriteCoord(vecOrigin.x);
	g_engfuncs.pfnWriteCoord(vecOrigin.y);
	g_engfuncs.pfnWriteCoord(vecOrigin.z + flRadius);

	g_engfuncs.pfnWriteShort(iSprite);
	g_engfuncs.pfnWriteByte(iStartingFrame);
	g_engfuncs.pfnWriteByte(std::lroundf(flFrameRate * 10.f));
	g_engfuncs.pfnWriteByte(std::lroundf(flLife * 10.f));
	g_engfuncs.pfnWriteByte(std::lroundf(flLineWidth * 10.f));
	g_engfuncs.pfnWriteByte(std::lroundf(flNoiseAmp * 100.f));

	g_engfuncs.pfnWriteByte(Color.r);
	g_engfuncs.pfnWriteByte(Color.g);
	g_engfuncs.pfnWriteByte(Color.b);
	g_engfuncs.pfnWriteByte(iBrightness);

	g_engfuncs.pfnWriteByte(std::lroundf(flScrollSpeed * 10.f));

	g_engfuncs.pfnMessageEnd();
}

EXPORT void UTIL_DLight(Vector const& vecOrigin, float flRadiusInMeter, color24 color, float flTime, float flDecayMeterPerSec) noexcept
{
	g_engfuncs.pfnMessageBegin(MSG_PVS, SVC_TEMPENTITY, vecOrigin, nullptr);
	g_engfuncs.pfnWriteByte(TE_DLIGHT);
	g_engfuncs.pfnWriteCoord(vecOrigin.x);
	g_engfuncs.pfnWriteCoord(vecOrigin.y);
	g_engfuncs.pfnWriteCoord(vecOrigin.z);
	g_engfuncs.pfnWriteByte(std::clamp(std::lround(flRadiusInMeter / 0.0254 / 10.0), 0l, 255l));	// (radius in 10's)
	g_engfuncs.pfnWriteByte(color.r);
	g_engfuncs.pfnWriteByte(color.g);
	g_engfuncs.pfnWriteByte(color.b);
	//g_engfuncs.pfnWriteByte(iBrightness);
	g_engfuncs.pfnWriteByte(std::clamp(std::lroundf(flTime * 10.f), 0l, 255l));	// (life in 10's)
	g_engfuncs.pfnWriteByte(std::clamp(std::lround(flDecayMeterPerSec / 0.0254 / 10.0), 0l, 255l));	// (decay rate in 10's)
	g_engfuncs.pfnMessageEnd();
}

/*
* Part III: Model Helper
*/

// Get player head!
// Note that it must be a player model, as the bone index was fixed.
EXPORT [[nodiscard]]
Vector UTIL_GetHeadPosition(edict_t *pPlayer) noexcept
{
	static std::unordered_map<int, int> rgModelHeadIndex{};
	static Vector vecOrigin{}, vecAngles{};

	// -1 means model has no head.
	if (pPlayer->v.modelindex <= 0 || rgModelHeadIndex[pPlayer->v.modelindex] == -1)
		return Vector::Zero();

	[[unlikely]]
	if (rgModelHeadIndex[pPlayer->v.modelindex] == 0)
	{
		unsigned iCount = 0;
		auto const pstudiohdr = g_engfuncs.pfnGetModelPtr(pPlayer);

		for (auto pBone = (mstudiobone_t *)((std::uintptr_t)pstudiohdr + pstudiohdr->boneindex); iCount < pstudiohdr->numbones; ++iCount, ++pBone)
		{
			if (strstr(pBone->name, "Head"))
				break;
		}

		if (iCount >= pstudiohdr->numbones)
		{
			rgModelHeadIndex[pPlayer->v.modelindex] = -1;	// model contains no head.
			return Vector::Zero();
		}

		rgModelHeadIndex[pPlayer->v.modelindex] = iCount;
	}

	g_engfuncs.pfnGetBonePosition(pPlayer, rgModelHeadIndex[pPlayer->v.modelindex], vecOrigin, vecAngles);

	return vecOrigin;	// The vecAngles is discarded due to the fact that the engine didn't even impl it
}

EXPORT struct BodyEnumInfo_t
{
	int m_index{};
	int m_total{};
};

EXPORT [[nodiscard]]	// #UPDATE_AT_CPP23 constexpr goto
decltype(entvars_t::body) UTIL_CalcBody(std::span<BodyEnumInfo_t const> info) noexcept
{
	for (auto ret = 0; ret < std::numeric_limits<decltype(entvars_t::body)>::max(); ++ret)
	{
		for (auto base = 1; auto && [index, total] : info)
		{
			if (ret / base % total != index)
			{
				goto LAB_CONTINUE;
			}

			base *= total;
		}

		return ret;
	LAB_CONTINUE:;
	}

	return 0;
}

EXPORT inline float UTIL_SetController(uint8_t *const piController, mstudiobonecontroller_t *const pbonecontroller, double degree) noexcept
{
	// wrap 0..360 if it's a rotational controller
	if (pbonecontroller->type & (STUDIO_XR | STUDIO_YR | STUDIO_ZR))
	{
		// ugly hack, invert value if end < start
		if (pbonecontroller->end < pbonecontroller->start)
			degree = -degree;

		// does the controller not wrap?
		if (pbonecontroller->start + 359.0 >= pbonecontroller->end)
		{
			if (degree > ((pbonecontroller->start + pbonecontroller->end) / 2.0) + 180)
				degree = degree - 360;
			if (degree < ((pbonecontroller->start + pbonecontroller->end) / 2.0) - 180)
				degree = degree + 360;
		}
		else
		{
			if (degree > 360)
				degree = degree - (int)(degree / 360.0) * 360.0;
			else if (degree < 0)
				degree = degree + (int)((degree / -360.0) + 1) * 360.0;
		}
	}

	auto setting = round(255.0 * (degree - pbonecontroller->start) / (pbonecontroller->end - pbonecontroller->start));

	if (setting < 0)
		setting = 0;

	if (setting > 255)
		setting = 255;

	*piController = static_cast<uint8_t>(setting);

	return static_cast<float>(setting * (1.0 / 255.0) * (pbonecontroller->end - pbonecontroller->start) + pbonecontroller->start);
}

EXPORT inline float UTIL_SetController(edict_t *pEdict, uint8_t iController, double flValue) noexcept
{
	unsigned i{};
	auto const pstudiohdr = g_engfuncs.pfnGetModelPtr(pEdict);

	if (!pstudiohdr)
		return (float)flValue;

	auto pbonecontroller = (mstudiobonecontroller_t *)((uint8_t*)pstudiohdr + pstudiohdr->bonecontrollerindex);

	// find first controller that matches the index
	for (i = 0; i < pstudiohdr->numbonecontrollers; i++, pbonecontroller++)
	{
		if (pbonecontroller->index == iController)
			break;
	}

	if (i >= pstudiohdr->numbonecontrollers)
		return (float)flValue;

	return UTIL_SetController(&pEdict->v.controller[iController], pbonecontroller, flValue);
}

EXPORT std::pair<Vector, Vector> UTIL_ExtractBbox(edict_t *pEdict, int const iSequence) noexcept
{
	auto const pstudiohdr = g_engfuncs.pfnGetModelPtr(pEdict);

	if (!pstudiohdr)
		return {};

	auto const pseqdesc = (mstudioseqdesc_t *)((uint8_t *)pstudiohdr + pstudiohdr->seqindex);
	return { pseqdesc[iSequence].bbmin, pseqdesc[iSequence].bbmax };
}

// Credit: Sneaky@GlobalModders.net
// Source: BonePosFix AMXX module

EXPORT inline
[[nodiscard]]
Vector UTIL_GetAttachment(edict_t* pEdict, uint16_t iAttachment) noexcept
{
	Vector ret{};
	//pEdict->v.angles.pitch = -pEdict->v.angles.pitch;	// infamous quake swap.
	g_engfuncs.pfnGetAttachment(pEdict, iAttachment, ret, nullptr);	// the angles are actually not implemented.
	//pEdict->v.angles.pitch = -pEdict->v.angles.pitch;

	// Fix CS angles
	auto const vecLocalOrigin = ret -= pEdict->v.origin;
	auto const c = std::cos(pEdict->v.angles.yaw * (std::numbers::pi / 180));
	auto const s = std::sin(pEdict->v.angles.yaw * (std::numbers::pi / 180));

	ret.x = static_cast<vec_t>(vecLocalOrigin.x * c - vecLocalOrigin.y * s);
	ret.y = static_cast<vec_t>(vecLocalOrigin.y * c + vecLocalOrigin.x * s);

	return ret + pEdict->v.origin;
}

// Credit: Sneaky@GlobalModders.net
// Source: BonePosFix AMXX module
EXPORT inline
[[nodiscard]]
Vector UTIL_GetBonePosition(edict_t* pEdict, std::int32_t iBone) noexcept
{
	auto const iEntity = ent_cast<short>(pEdict);
	auto const bIsPlayer = iEntity >= 1 && iEntity <= gpGlobals->maxClients;

	Vector ret{};
	pEdict->v.angles[0] = -pEdict->v.angles[0];
	g_engfuncs.pfnGetBonePosition(pEdict, iBone, ret, nullptr);
	pEdict->v.angles[0] = -pEdict->v.angles[0];

	if (!bIsPlayer)
	{
		auto const vecLocalOrigin = ret -= pEdict->v.origin;
		auto const c = std::cos(pEdict->v.angles.yaw * (std::numbers::pi / 180));
		auto const s = std::sin(pEdict->v.angles.yaw * (std::numbers::pi / 180));

		ret.x = static_cast<vec_t>(vecLocalOrigin.x * c - vecLocalOrigin.y * s);
		ret.y = static_cast<vec_t>(vecLocalOrigin.y * c + vecLocalOrigin.x * s);

		return ret + pEdict->v.origin;
	}

	return ret;
}
