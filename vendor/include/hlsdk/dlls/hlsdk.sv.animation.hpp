#if !defined(INCLUDED_IN_MODULE) || defined(__INTELLISENSE__)
#pragma once

#include <cstdint>
#include <cstddef>

#include <ranges>
#include <string_view>

#include "../engine/hlsdk.engine.hpp"
#include "../dlls/hlsdk.sv.hpp"

#else

import std;

import :engine;
import :sv;

#endif

#ifndef EXPORT
#define EXPORT
#endif

#pragma region r_studioint.h
EXPORT inline constexpr auto STUDIO_INTERFACE_VERSION = 1;

EXPORT struct engine_studio_api_t
{
	// Allocate number*size bytes and zero it
	void* (*Mem_Calloc)(int number, size_t size) noexcept {};

	// Check to see if pointer is in the cache
	void* (*Cache_Check)(struct cache_user_s* c) noexcept {};

	// Load file into cache (can be swapped out on demand)
	void (*LoadCacheFile)(char const* path, struct cache_user_s* cu) noexcept {};

	// Retrieve model pointer for the named model
	struct model_s* (*Mod_ForName)(const char* name, int crash_if_missing) noexcept {};

	// Retrieve pointer to studio model data block from a model
	void* (*Mod_Extradata)(model_t* mod) noexcept {};

	// Retrieve indexed model from client side model precache list
	struct model_s* (*GetModelByIndex)(int index) noexcept {};

	// Get entity that is set for rendering
	struct cl_entity_s* (*GetCurrentEntity)(void) noexcept {};

	// Get referenced player_info_t
	struct player_info_s* (*PlayerInfo)(int index) noexcept {};

	// Get most recently received player state data from network system
	struct entity_state_s* (*GetPlayerState)(int index) noexcept {};

	// Get viewentity
	struct cl_entity_s* (*GetViewEntity)(void) noexcept {};

	// Get current frame count, and last two timestampes on client
	void (*GetTimes)(int* framecount, double* current, double* old) noexcept {};

	// Get a pointer to a cvar by name
	struct cvar_s* (*GetCvar)(const char* name) noexcept {};

	// Get current render origin and view vectors (up, right and vpn)
	void (*GetViewInfo)(float* origin, float* upv, float* rightv, float* vpnv) noexcept {};

	// Get sprite model used for applying chrome effect
	struct model_s* (*GetChromeSprite)(void) noexcept {};

	// Get model counters so we can incement instrumentation
	void (*GetModelCounters)(int** s, int** a) noexcept {};

	// Get software scaling coefficients
	void (*GetAliasScale)(float* x, float* y) noexcept {};

	// Get bone, light, alias, and rotation matrices
	float**** (*StudioGetBoneTransform) (void) noexcept {};
	float**** (*StudioGetLightTransform)(void) noexcept {};
	float*** (*StudioGetAliasTransform) (void) noexcept {};
	float*** (*StudioGetRotationMatrix) (void) noexcept {};

	// Set up body part, and get submodel pointers
	void (*StudioSetupModel)(int bodypart, void** ppbodypart, void** ppsubmodel) noexcept {};

	// Check if entity's bbox is in the view frustum
	int (*StudioCheckBBox)(void) noexcept {};

	// Apply lighting effects to model
	void (*StudioDynamicLight)(struct cl_entity_s* ent, struct alight_s* plight) noexcept {};
	void (*StudioEntityLight)(struct alight_s* plight) noexcept {};
	void (*StudioSetupLighting)(struct alight_s* plighting) noexcept {};

	// Draw mesh vertices
	void (*StudioDrawPoints)(void) noexcept {};

	// Draw hulls around bones
	void (*StudioDrawHulls)(void) noexcept {};

	// Draw bbox around studio models
	void (*StudioDrawAbsBBox)(void) noexcept {};

	// Draws bones
	void (*StudioDrawBones)(void) noexcept {};
	// Loads in appropriate texture for model

	void (*StudioSetupSkin)(void* ptexturehdr, int index) noexcept {};

	// Sets up for remapped colors
	void (*StudioSetRemapColors)(int top, int bottom) noexcept {};

	// Set's player model and returns model pointer
	struct model_s* (*SetupPlayerModel)(int index) noexcept {};

	// Fires any events embedded in animation
	void (*StudioClientEvents)(void) noexcept {};

	// Retrieve/set forced render effects flags
	int (*GetForceFaceFlags)(void) noexcept {};
	void (*SetForceFaceFlags)(int flags) noexcept {};

	// Tell engine the value of the studio model header
	void (*StudioSetHeader)(void* header) noexcept {};

	// Tell engine which model_t * is being renderered
	void (*SetRenderModel)(struct model_s* model) noexcept {};

	// Final state setup and restore for rendering
	void (*SetupRenderer)(int rendermode) noexcept {};
	void (*RestoreRenderer)(void) noexcept {};

	// Set render origin for applying chrome effect
	void (*SetChromeOrigin)(void) noexcept {};

	// True if using D3D/OpenGL
	int (*IsHardware)(void) noexcept {};

	// Only called by hardware interface
	void (*GL_StudioDrawShadow)(void) noexcept {};
	void (*GL_SetRenderMode)(int mode) noexcept {};

	void (*StudioSetRenderamt)(int iRenderamt) noexcept {};	//!!!CZERO added for rendering glass on viewmodels
	void (*StudioSetCullState)(int iCull) noexcept {};
	void (*StudioRenderShadow)(int iSprite, float* p1, float* p2, float* p3, float* p4) noexcept {};
};

EXPORT struct server_studio_api_t
{
	// Allocate number*size bytes and zero it
	void* (*Mem_Calloc)(int number, size_t size) noexcept {};

	// Check to see if pointer is in the cache
	void* (*Cache_Check)(struct cache_user_s* c) noexcept {};

	// Load file into cache (can be swapped out on demand)
	void (*LoadCacheFile)(char* path, struct cache_user_s* cu) noexcept {};

	// Retrieve pointer to studio model data block from a model
	studiohdr_t* (*Mod_Extradata)(model_t* mod) noexcept {};
};


// client blending
EXPORT struct r_studio_interface_t
{
	int version{ STUDIO_INTERFACE_VERSION };
	int (*StudioDrawModel)(int flags) noexcept {};
	int (*StudioDrawPlayer)(int flags, struct entity_state_s* pplayer) noexcept {};
};

EXPORT extern "C++" inline r_studio_interface_t* gpStudioAPI{};

// server blending
EXPORT inline constexpr auto SV_BLENDING_INTERFACE_VERSION = 1;

EXPORT struct sv_blending_interface_t
{
	int version{ SV_BLENDING_INTERFACE_VERSION };

	void (*SV_StudioSetupBones)(
		model_t* pModel,
		float frame,
		int sequence,
		const float* angles,
		const float* origin,
		const uint8_t* pcontroller,	// Actual uint8, not bytes. Same below.
		const uint8_t* pblending,
		int iBone,
		const edict_t* pEdict
		) noexcept {};
};
#pragma endregion r_studioint.h

EXPORT inline auto ExtractBbox(studiohdr_t* pmodel, int sequence) noexcept -> std::pair<Vector, Vector>
{
	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex);

	return {
		pseqdesc[sequence].bbmin,
		pseqdesc[sequence].bbmax,
	};
}

EXPORT inline int LookupActivity(studiohdr_t* pmodel, Activity activity) noexcept
{
	unsigned i{};
	int weightTotal = 0;
	int activitySequenceCount = 0;
	int weight = 0;

	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex);

	for (i = 0; i < pmodel->numseq; i++)
	{
		if (pseqdesc[i].activity == activity)
		{
			weightTotal += pseqdesc[i].actweight;
			++activitySequenceCount;
		}
	}

	if (activitySequenceCount > 0)
	{
		if (weightTotal)
		{
			int which = g_engfuncs.pfnRandomLong(0, weightTotal - 1);

			for (i = 0; i < pmodel->numseq; i++)
			{
				if (pseqdesc[i].activity == activity)
				{
					weight += pseqdesc[i].actweight;

					if (weight > which)
					{
						return i;
					}
				}
			}
		}
		else
		{
			auto select = g_engfuncs.pfnRandomLong(0, activitySequenceCount - 1);

			for (i = 0; i < pmodel->numseq; i++)
			{
				if (pseqdesc[i].activity == activity)
				{
					if (select == 0)
					{
						return i;
					}

					--select;
				}
			}
		}
	}

	return -1;	// ACT_INVALID
}

EXPORT inline int LookupActivityHeaviest(studiohdr_t* pmodel, Activity activity) noexcept
{
	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex);

	int weight = 0;
	int seq = -1;	// ACTIVITY_NOT_AVAILABLE

	for (auto i = 0u; i < pmodel->numseq; i++)
	{
		if (pseqdesc[i].activity == activity)
		{
			if (pseqdesc[i].actweight > weight)
			{
				weight = pseqdesc[i].actweight;
				seq = i;
			}
		}
	}

	return seq;
}

EXPORT inline Vector GetEyePosition(studiohdr_t* pmodel) noexcept
{
	if (!pmodel)
	{
		g_engfuncs.pfnAlertMessage(at_console, "GetEyePosition() Can't get pstudiohdr ptr!\n");
		return Vector{};
	}

	return pmodel->eyeposition;
}

EXPORT inline int LookupSequence(studiohdr_t* pmodel, const char* label) noexcept
{
	static constexpr auto StrICmp =
		[](char const* s1, char const* s2) noexcept
		{
			while (*s1 && (*s1 == *s2))
			{
				++s1;
				++s2;
			}

			return *(const unsigned char*)s1 - *(const unsigned char*)s2;
		};

	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex);

	for (auto i = 0u; i < pmodel->numseq; ++i)
	{
		if (StrICmp(pseqdesc[i].label, label) == 0)
			return i;
	}

	return -1;
}

EXPORT inline bool IsSoundEvent(int eventNumber)
{
	if (eventNumber == SCRIPT_EVENT_SOUND || eventNumber == SCRIPT_EVENT_SOUND_VOICE)
		return true;

	return false;
}

EXPORT inline void SequencePrecache(studiohdr_t* pmodel, const char* pSequenceName) noexcept
{
	if (auto const index = LookupSequence(pmodel, pSequenceName); index >= 0)
	{
		auto const pseqdesc = (mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex) + index;
		auto const pevent = (mstudioevent_t*)((std::byte*)pmodel + pseqdesc->eventindex);

		for (unsigned i = 0; i < pseqdesc->numevents; ++i)
		{
			// Don't send client-side events to the server AI
			if (pevent[i].event >= EVENT_CLIENT)
				continue;

			// UNDONE: Add a callback to check to see if a sound is precached yet and don't allocate a copy
			// of it's name if it is.
			if (IsSoundEvent(pevent[i].event))
			{
				if (!std::strlen(pevent[i].options))
				{
					g_engfuncs.pfnAlertMessage(
						at_error,
						"Bad sound event %d in sequence %s :: %s (sound is \"%s\")\n",
						pevent[i].event, pmodel->name, pSequenceName, pevent[i].options
					);
				}

				// Precache requires static-stored string.
				// #SHOULD_DO_ON_FREE Technically leaked. Consider a std::set<string> instead?
				g_engfuncs.pfnPrecacheSound(
					(char const*)(gpGlobals->pStringBase + g_engfuncs.pfnAllocString(pevent[i].options))
				);
			}
		}
	}
}

EXPORT inline void GetSequenceInfo(studiohdr_t* pmodel, int seq, float* pflFrameRate, float* pflGroundSpeed) noexcept
{
	if (seq >= (signed)pmodel->numseq)
	{
		*pflFrameRate = 0;
		*pflGroundSpeed = 0;
		return;
	}

	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex) + seq;

	if (pseqdesc->numframes <= 1)
	{
		*pflFrameRate = 256.0f;
		*pflGroundSpeed = 0.0f;
		return;
	}

	*pflFrameRate = pseqdesc->fps * 256.0f / float(pseqdesc->numframes - 1);
	*pflGroundSpeed = (float)pseqdesc->linearmovement.Length();
	*pflGroundSpeed *= pseqdesc->fps / float(pseqdesc->numframes - 1);
}

EXPORT inline int GetSequenceFlags(studiohdr_t* pmodel, int seq) noexcept
{
	if (!pmodel || seq >= (signed)pmodel->numseq)
		return 0;

	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex) + seq;

	return pseqdesc->flags;
}

EXPORT inline int GetAnimationEvent(studiohdr_t* pmodel, int seq, MonsterEvent_t* pMonsterEvent, float flStart, float flEnd, int index) noexcept
{
	if (!pmodel || seq >= (signed)pmodel->numseq || !pMonsterEvent)
	{
		return 0;
	}

	auto const pseqdesc = (mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex) + seq;
	auto const pevent = (mstudioevent_t*)((std::byte*)pmodel + pseqdesc->eventindex);

	if (pseqdesc->numevents == 0 || index > (signed)pseqdesc->numevents)
	{
		return 0;
	}

	if (pseqdesc->numframes > 1)
	{
		flStart *= static_cast<float>((pseqdesc->numframes - 1) / 256.0);
		flEnd *= static_cast<float>((pseqdesc->numframes - 1) / 256.0);
	}
	else
	{
		flStart = 0.f;
		flEnd = 1.f;
	}

	for (; index < (signed)pseqdesc->numevents; ++index)
	{
		// Don't send client-side events to the server AI
		if (pevent[index].event >= EVENT_CLIENT)
			continue;

		if ((pevent[index].frame >= flStart && pevent[index].frame < flEnd) ||
			((pseqdesc->flags & STUDIO_LOOPING)
				&& flEnd >= pseqdesc->numframes - 1
				&& pevent[index].frame < flEnd - pseqdesc->numframes + 1))
		{
			pMonsterEvent->event = pevent[index].event;
			pMonsterEvent->options = pevent[index].options;

			return index + 1;
		}
	}

	return 0;
}

// LUNA: provided in sv.util.hpp already.
//float SetController(void* pmodel, entvars_t* pev, int iController, float flValue);

EXPORT inline float SetBlending(entvars_t* pev, int iBlender, float flValue) noexcept
{
	auto const pModel = g_engfuncs.pfnGetModelPtr(pev->pContainingEntity);
	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pModel + pModel->seqindex);

	if (pseqdesc->blendtype[iBlender] == 0)
		return flValue;

	if (pseqdesc->blendtype[iBlender] & (STUDIO_XR | STUDIO_YR | STUDIO_ZR))
	{
		// ugly hack, invert value if end < start
		if (pseqdesc->blendend[iBlender] < pseqdesc->blendstart[iBlender])
			flValue = -flValue;

		// does the controller not wrap?
		if (pseqdesc->blendstart[iBlender] + 359.0 >= pseqdesc->blendend[iBlender])
		{
			if (flValue > ((pseqdesc->blendstart[iBlender] + pseqdesc->blendend[iBlender]) / 2.0) + 180)
				flValue = flValue - 360;
			if (flValue < ((pseqdesc->blendstart[iBlender] + pseqdesc->blendend[iBlender]) / 2.0) - 180)
				flValue = flValue + 360;
		}
	}

	auto const setting = std::clamp(
		static_cast<int>(255.0 * (flValue - pseqdesc->blendstart[iBlender]) / (pseqdesc->blendend[iBlender] - pseqdesc->blendstart[iBlender])),
		0, 255
	);

	pev->blending[iBlender] = (std::uint8_t)setting;

	return
		setting * (1.f / 255.f) * (pseqdesc->blendend[iBlender] - pseqdesc->blendstart[iBlender]) + pseqdesc->blendstart[iBlender];
}

EXPORT inline int FindTransition(studiohdr_t* pmodel, int iEndingAnim, int iGoalAnim, int* piDir) noexcept
{
	auto const pseqdesc =
		(mstudioseqdesc_t*)((std::byte*)pmodel + pmodel->seqindex);

	// bail if we're going to or from a node 0
	if (pseqdesc[iEndingAnim].entrynode == 0 || pseqdesc[iGoalAnim].entrynode == 0)
	{
		return iGoalAnim;
	}

	int	iEndNode{};

	// ALERT( at_console, "from %d to %d: ", pEndNode->iEndNode, pGoalNode->iStartNode );

	if (*piDir > 0)
	{
		iEndNode = pseqdesc[iEndingAnim].exitnode;
	}
	else
	{
		iEndNode = pseqdesc[iEndingAnim].entrynode;
	}

	if (iEndNode == pseqdesc[iGoalAnim].entrynode)
	{
		*piDir = 1;
		return iGoalAnim;
	}

	auto const pTransition = ((std::byte*)pmodel + pmodel->transitionindex);
	auto const iInternNode = pTransition[(iEndNode - 1) * pmodel->numtransitions + (pseqdesc[iGoalAnim].entrynode - 1)];

	if ((std::uint8_t)iInternNode == 0)
		return iGoalAnim;

	// look for someone going
	for (auto i = 0u; i < pmodel->numseq; i++)
	{
		if (pseqdesc[i].entrynode == iEndNode && pseqdesc[i].exitnode == (std::uint8_t)iInternNode)
		{
			*piDir = 1;
			return i;
		}
		if (pseqdesc[i].nodeflags)
		{
			if (pseqdesc[i].exitnode == iEndNode && pseqdesc[i].entrynode == (std::uint8_t)iInternNode)
			{
				*piDir = -1;
				return i;
			}
		}
	}

	g_engfuncs.pfnAlertMessage(at_console, "error in transition graph");
	return iGoalAnim;
}

// LUNA: Consider UTIL_CalcBody() as well.
EXPORT inline void SetBodygroup(entvars_t* pev, int iGroup, int iValue) noexcept
{
	auto const pModel = g_engfuncs.pfnGetModelPtr(pev->pContainingEntity);

	if (iGroup > (signed)pModel->numbodyparts)
		return;

	auto const pbodypart =
		(mstudiobodyparts_t*)((std::byte*)pModel + pModel->bodypartindex) + iGroup;

	if (iValue >= (int)pbodypart->nummodels)
		return;

	auto const iCurrent = (pev->body / pbodypart->base) % pbodypart->nummodels;

	pev->body = (pev->body - (iCurrent * pbodypart->base) + (iValue * pbodypart->base));
}

EXPORT inline int GetBodygroup(entvars_t* pev, int iGroup) noexcept
{
	auto const pModel = g_engfuncs.pfnGetModelPtr(pev->pContainingEntity);

	if (iGroup > (signed)pModel->numbodyparts)
		return 0;

	auto const pbodypart =
		(mstudiobodyparts_t*)((std::byte*)pModel + pModel->bodypartindex) + iGroup;

	if (pbodypart->nummodels <= 1)
		return 0;

	return (pev->body / pbodypart->base) % pbodypart->nummodels;
}
