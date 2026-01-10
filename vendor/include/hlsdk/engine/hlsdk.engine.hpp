#if !defined(INCLUDED_IN_MODULE) || defined(__INTELLISENSE__)
#pragma once

#include <cstdint>
#include <cstddef>

#include <numbers>

#include "../dlls/vector.hpp"

#else

import std.compat;

import vector;

#endif

#ifndef EXPORT
#define EXPORT
#endif


#pragma region const.h

//
// Constants shared by the engine and dlls
// This header file included by engine files and DLL files.
// Most came from server.h

// edict->flags
EXPORT enum EVAR_FLAGS : uint32_t
{
	FL_FLY = (1 << 0),				// Changes the SV_Movestep() behavior to not need to be on ground
	FL_SWIM = (1 << 1),				// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
	FL_CONVEYOR = (1 << 2),
	FL_CLIENT = (1 << 3),
	FL_INWATER = (1 << 4),
	FL_MONSTER = (1 << 5),
	FL_GODMODE = (1 << 6),
	FL_NOTARGET = (1 << 7),
	FL_SKIPLOCALHOST = (1 << 8),	// Don't send entity to local host, it's predicting this entity itself
	FL_ONGROUND = (1 << 9),			// At rest / on the ground
	FL_PARTIALGROUND = (1 << 10),	// not all corners are valid
	FL_WATERJUMP = (1 << 11),		// player jumping out of water
	FL_FROZEN = (1 << 12),			// Player is frozen for 3rd person camera
	FL_FAKECLIENT = (1 << 13),		// JAC: fake client, simulated server side, don't send network messages to them
	FL_DUCKING = (1 << 14),			// Player flag -- Player is fully crouched
	FL_FLOAT = (1 << 15),			// Apply floating force to this entity when in water
	FL_GRAPHED = (1 << 16),			// worldgraph has this ent listed as something that blocks a connection

	// UNDONE: Do we need these?
	FL_IMMUNE_WATER = (1 << 17),
	FL_IMMUNE_SLIME = (1 << 18),
	FL_IMMUNE_LAVA = (1 << 19),

	FL_PROXY = (1 << 20),			// This is a spectator proxy
	FL_ALWAYSTHINK = (1 << 21),		// Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path)
	FL_BASEVELOCITY = (1 << 22),	// Base velocity has been applied this frame (used to convert base velocity into momentum)
	FL_MONSTERCLIP = (1 << 23),		// Only collide in with monsters who have FL_MONSTERCLIP set
	FL_ONTRAIN = (1 << 24),			// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
	FL_WORLDBRUSH = (1 << 25),		// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
	FL_SPECTATOR = (1 << 26),		// This client is a spectator, don't run touch functions, etc.
	FL_CUSTOMENTITY = (1 << 29),	// This is a custom entity
	FL_KILLME = (1 << 30),			// This entity is marked for death -- This allows the engine to kill ents at the appropriate time
	FL_DORMANT = (1u << 31),			// Entity is dormant, no updates to client
};

// Goes into globalvars_t.trace_flags
EXPORT inline constexpr auto FTRACE_SIMPLEBOX = (1 << 0);	// Traceline with a simple box

// walkmove modes
EXPORT enum WALKMOVE : uint32_t
{
	WALKMOVE_NORMAL = 0,		// normal walkmove
	WALKMOVE_WORLDONLY = 1,		// doesn't hit ANY entities, no matter what the solid type
	WALKMOVE_CHECKONLY = 2,		// move, but don't touch triggers
};

// edict->movetype values
EXPORT enum EVAR_MT : uint32_t
{
	MOVETYPE_NONE = 0,				// never moves
	MOVETYPE_ANGLENOCLIP [[deprecated]] = 1,
	MOVETYPE_ANGLECLIP [[deprecated]] = 2,
	MOVETYPE_WALK = 3,				// Player only - moving on the ground
	MOVETYPE_STEP = 4,				// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY = 5,				// No gravity, but still collides with stuff
	MOVETYPE_TOSS = 6,				// gravity/collisions
	MOVETYPE_PUSH = 7,				// no clip to world, push and crush
	MOVETYPE_NOCLIP = 8,			// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_FLYMISSILE = 9,		// extra size to monsters
	MOVETYPE_BOUNCE = 10,			// Just like Toss, but reflect velocity when contacting surfaces
	MOVETYPE_BOUNCEMISSILE = 11,	// bounce w/o gravity
	MOVETYPE_FOLLOW = 12,			// track movement of aiment
	MOVETYPE_PUSHSTEP = 13,			// BSP model that needs physics/world collisions (uses nearest hull for world collision)
};

// edict->solid values
// NOTE: Some movetypes will cause collisions independent of SOLID_NOT/SOLID_TRIGGER when the entity moves
// SOLID only effects OTHER entities colliding with this one when they move - UGH!
EXPORT enum EVAR_SOLID : uint32_t
{
	SOLID_NOT = 0,			// no interaction with other objects
	SOLID_TRIGGER = 1,		// touch on edge, but not blocking
	SOLID_BBOX = 2,			// touch on edge, block
	SOLID_SLIDEBOX = 3,		// touch on edge, but not an onground
	SOLID_BSP = 4,			// bsp clip, touch on edge, block
};

// edict->deadflag values
EXPORT enum EVAR_DF : uint32_t
{
	DEAD_NO = 0,			// alive
	DEAD_DYING = 1,			// playing death animation or still falling off of a ledge waiting to hit ground
	DEAD_DEAD = 2,			// dead. lying still.
	DEAD_RESPAWNABLE = 3,	// do respawn the entity
	DEAD_DISCARDBODY = 4,
};

// edict->takedamage
// decltype(entvars_t::takedamage) == float
EXPORT inline constexpr float DAMAGE_NO = 0;
EXPORT inline constexpr float DAMAGE_YES = 1;
EXPORT inline constexpr float DAMAGE_AIM = 2;

// edict->effects values
EXPORT enum EVAR_EFFECTS : uint32_t
{
	EF_BRIGHTFIELD = 1,	// swirling cloud of particles
	EF_MUZZLEFLASH = 2,	// single frame ELIGHT on entity attachment 0
	EF_BRIGHTLIGHT = 4,	// DLIGHT centered at entity origin
	EF_DIMLIGHT = 8,	// player flashlight
	EF_INVLIGHT = 16,	// get lighting from ceiling
	EF_NOINTERP = 32,	// don't interpolate the next frame
	EF_LIGHT = 64,		// rocket flare glow sprite
	EF_NODRAW = 128,	// don't draw entity
};

// state->eflags values
EXPORT inline constexpr uint32_t EFLAG_SLERP = 1;	// do studio interpolation of this entity

// pev->gamestate values
EXPORT enum EVAR_HGS : uint32_t
{
	HITGROUP_SHIELD_ENABLED = 0,
	HITGROUP_SHIELD_DISABLED = 1,
};

//
// temp entity events
// LUNA: refer to https://github.com/baso88/SC_AngelScript/wiki/Temporary-Entities for more detail.
//
EXPORT enum SVC_TEMPENTITY_TYPES : uint8_t
{
	TE_BEAMPOINTS = 0,		// beam effect between two points
	// coord coord coord (start position) 
	// coord coord coord (end position) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_BEAMENTPOINT = 1,		// beam effect between point and entity
	// short (start entity) 
	// coord coord coord (end position) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_GUNSHOT = 2,		// particle effect plus ricochet sound
	// coord coord coord (position) 

	TE_EXPLOSION = 3,		// additive sprite, 2 dynamic lights, flickering particles, explosion sound, move vertically 8 pps
	// coord coord coord (position) 
	// short (sprite index)
	// byte (scale in 0.1's)
	// byte (framerate)
	// byte (flags)
	//
	// The Explosion effect has some flags to control performance/aesthetic features:
	// LUNA: check enumerator TE_EXPLFLAGS.


	TE_TAREXPLOSION = 4,		// Quake1 "tarbaby" explosion with sound
	// coord coord coord (position) 

	TE_SMOKE = 5,		// alphablend sprite, move vertically 30 pps
	// coord coord coord (position) 
	// short (sprite index)
	// byte (scale in 0.1's)
	// byte (framerate)

	TE_TRACER = 6,		// tracer effect from point to point
	// coord, coord, coord (start) 
	// coord, coord, coord (end)

	TE_LIGHTNING = 7,		// TE_BEAMPOINTS with simplified parameters
	// coord, coord, coord (start) 
	// coord, coord, coord (end) 
	// byte (life in 0.1's) 
	// byte (width in 0.1's) 
	// byte (amplitude in 0.01's)
	// short (sprite model index)

	TE_BEAMENTS = 8,
	// short (start entity) 
	// short (end entity) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_SPARKS = 9,		// 8 random tracers with gravity, ricochet sprite
	// coord coord coord (position) 

	TE_LAVASPLASH = 10,		// Quake1 lava splash
	// coord coord coord (position) 

	TE_TELEPORT = 11,		// Quake1 teleport splash
	// coord coord coord (position) 

	TE_EXPLOSION2 = 12,		// Quake1 colormaped (base palette) particle explosion with sound
	// coord coord coord (position) 
	// byte (starting color)
	// byte (num colors)

	TE_BSPDECAL = 13,		// Decal from the .BSP file 
	// coord, coord, coord (x,y,z), decal position (center of texture in world)
	// short (texture index of precached decal texture name)
	// short (entity index)
	// [optional - only included if previous short is non-zero (not the world)] short (index of model of above entity)

	TE_IMPLOSION = 14,		// tracers moving toward a point
	// coord, coord, coord (position)
	// byte (radius)
	// byte (count)
	// byte (life in 0.1's) 

	TE_SPRITETRAIL = 15,		// line of moving glow sprites with gravity, fadeout, and collisions
	// coord, coord, coord (start) 
	// coord, coord, coord (end) 
	// short (sprite index)
	// byte (count)
	// byte (life in 0.1's) 
	// byte (scale in 0.1's) 
	// byte (velocity along vector in 10's)
	// byte (randomness of velocity in 10's)

	TE_BEAM [[deprecated]] = 16,		// obsolete

	TE_SPRITE = 17,		// additive sprite, plays 1 cycle
	// coord, coord, coord (position) 
	// short (sprite index) 
	// byte (scale in 0.1's) 
	// byte (brightness)

	TE_BEAMSPRITE = 18,		// A beam with a sprite at the end
	// coord, coord, coord (start position) 
	// coord, coord, coord (end position) 
	// short (beam sprite index) 
	// short (end sprite index) 

	TE_BEAMTORUS = 19,		// screen aligned beam ring, expands to max radius over lifetime
	// coord coord coord (center position) 
	// coord coord coord (axis and radius) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_BEAMDISK = 20,		// disk that expands to max radius over lifetime
	// coord coord coord (center position) 
	// coord coord coord (axis and radius) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_BEAMCYLINDER = 21,		// cylinder that expands to max radius over lifetime
	// coord coord coord (center position) 
	// coord coord coord (axis and radius) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_BEAMFOLLOW = 22,		// create a line of decaying beam segments until entity stops moving
	// short (entity:attachment to follow)
	// short (sprite index)
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte,byte,byte (color)
	// byte (brightness)

	TE_GLOWSPRITE = 23,
	// coord, coord, coord (pos) short (model index) byte (scale / 10)

	TE_BEAMRING = 24,		// connect a beam ring to two entities
	// short (start entity) 
	// short (end entity) 
	// short (sprite index) 
	// byte (starting frame) 
	// byte (frame rate in 0.1's) 
	// byte (life in 0.1's) 
	// byte (line width in 0.1's) 
	// byte (noise amplitude in 0.01's) 
	// byte,byte,byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	TE_STREAK_SPLASH = 25,		// oriented shower of tracers
	// coord coord coord (start position) 
	// coord coord coord (direction vector) 
	// byte (color)
	// short (count)
	// short (base speed)
	// short (ramdon velocity)

	TE_BEAMHOSE [[deprecated]] = 26,		// obsolete

	TE_DLIGHT = 27,		// dynamic light, effect world, minor entity effect
	// coord, coord, coord (pos) 
	// byte (radius in 10's) 
	// byte byte byte (color)
	// byte (brightness) - LUNA: WRONG! no such argument!
	// byte (life in 10's) - LUNA: it's in 0.1's
	// byte (decay rate in 10's)

	TE_ELIGHT = 28,		// point entity light, no world effect
	// short (entity:attachment to follow)
	// coord coord coord (initial position) 
	// coord (radius)
	// byte byte byte (color)
	// byte (life in 0.1's)
	// coord (decay rate)

	TE_TEXTMESSAGE = 29,
	// short 1.2.13 x (-1 = center)
	// short 1.2.13 y (-1 = center)
	// byte Effect 0 = fade in/fade out
				// 1 is flickery credits
				// 2 is write out (training room)

	// 4 bytes r,g,b,a color1	(text color)
	// 4 bytes r,g,b,a color2	(effect color)
	// ushort 8.8 fadein time
	// ushort 8.8 fadeout time
	// ushort 8.8 hold time
	// optional ushort 8.8 fxtime	(time the highlight lags behing the leading text in effect 2)
	// string text message		(512 chars max sz string)

	TE_LINE = 30,
	// coord, coord, coord		startpos
	// coord, coord, coord		endpos
	// short life in 0.1 s
	// 3 bytes r, g, b

	TE_BOX = 31,
	// coord, coord, coord		boxmins
	// coord, coord, coord		boxmaxs
	// short life in 0.1 s
	// 3 bytes r, g, b

	TE_KILLBEAM = 99,		// kill all beams attached to entity
	// short (entity)

	TE_LARGEFUNNEL = 100,
	// coord coord coord (funnel position)
	// short (sprite index) 
	// short (flags) 

	TE_BLOODSTREAM = 101,		// particle spray
	// coord coord coord (start position)
	// coord coord coord (spray vector)
	// byte (color)
	// byte (speed)

	TE_SHOWLINE = 102,		// line of particles every 5 units, dies in 30 seconds
	// coord coord coord (start position)
	// coord coord coord (end position)

	TE_BLOOD = 103,		// particle spray
	// coord coord coord (start position)
	// coord coord coord (spray vector)
	// byte (color)
	// byte (speed)

	TE_DECAL = 104,		// Decal applied to a brush entity (not the world)
	// coord, coord, coord (x,y,z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name)
	// short (entity index)

	TE_FIZZ = 105,		// create alpha sprites inside of entity, float upwards
	// short (entity)
	// short (sprite index)
	// byte (density)

	TE_MODEL = 106,		// create a moving model that bounces and makes a sound when it hits
	// coord, coord, coord (position) 
	// coord, coord, coord (velocity)
	// angle (initial yaw)
	// short (model index)
	// byte (bounce sound type)
	// byte (life in 0.1's)

	TE_EXPLODEMODEL = 107,		// spherical shower of models, picks from set
	// coord, coord, coord (origin)
	// coord (velocity)
	// short (model index)
	// short (count)
	// byte (life in 0.1's)

	TE_BREAKMODEL = 108,		// box of models or sprites
	// coord, coord, coord (position)
	// coord, coord, coord (size)
	// coord, coord, coord (velocity)
	// byte (random velocity in 10's)
	// short (sprite or model index)
	// byte (count)
	// byte (life in 0.1 secs)
	// byte (flags)

	TE_GUNSHOTDECAL = 109,		// decal and ricochet sound
	// coord, coord, coord (position)
	// short (entity index???)
	// byte (decal???)

	TE_SPRITE_SPRAY = 110,		// spay of alpha sprites
	// coord, coord, coord (position)
	// coord, coord, coord (velocity)
	// short (sprite index)
	// byte (count)
	// byte (speed)
	// byte (noise)

	TE_ARMOR_RICOCHET = 111,		// quick spark sprite, client ricochet sound. 
	// coord, coord, coord (position)
	// byte (scale in 0.1's)

	TE_PLAYERDECAL = 112,		// ???
	// byte (playerindex)
	// coord, coord, coord (position)
	// short (entity???)
	// byte (decal number???)
	// [optional] short (model index???)

	TE_BUBBLES = 113,		// create alpha sprites inside of box, float upwards
	// coord, coord, coord (min start position)
	// coord, coord, coord (max start position)
	// coord (float height)
	// short (model index)
	// byte (count)
	// coord (speed)

	TE_BUBBLETRAIL = 114,		// create alpha sprites along a line, float upwards
	// coord, coord, coord (min start position)
	// coord, coord, coord (max start position)
	// coord (float height)
	// short (model index)
	// byte (count)
	// coord (speed)

	TE_BLOODSPRITE = 115,		// spray of opaque sprite1's that fall, single sprite2 for 1..2 secs (this is a high-priority tent)
	// coord, coord, coord (position)
	// short (sprite1 index)
	// short (sprite2 index)
	// byte (color)
	// byte (scale)

	TE_WORLDDECAL = 116,		// Decal applied to the world brush
	// coord, coord, coord (x,y,z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name)

	TE_WORLDDECALHIGH = 117,		// Decal (with texture index > 256) applied to world brush
	// coord, coord, coord (x,y,z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name - 256)

	TE_DECALHIGH = 118,		// Same as TE_DECAL, but the texture index was greater than 256
	// coord, coord, coord (x,y,z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name - 256)
	// short (entity index)

	TE_PROJECTILE = 119,		// Makes a projectile (like a nail) (this is a high-priority tent)
	// coord, coord, coord (position)
	// coord, coord, coord (velocity)
	// short (modelindex)
	// byte (life)
	// byte (owner)  projectile won't collide with owner (if owner == 0, projectile will hit any client).

	TE_SPRAY = 120,		// Throws a shower of sprites or models
	// coord, coord, coord (position)
	// coord, coord, coord (direction)
	// short (modelindex)
	// byte (count)
	// byte (speed)
	// byte (noise)
	// byte (rendermode)

	TE_PLAYERSPRITES = 121,		// sprites emit from a player's bounding box (ONLY use for players!)
	// byte (playernum)
	// short (sprite modelindex)
	// byte (count)
	// byte (variance) (0 = no variance in size) (10 = 10% variance in size)

	TE_PARTICLEBURST = 122,		// very similar to lavasplash.
	// coord (origin)
	// short (radius)
	// byte (particle color)
	// byte (duration * 10) (will be randomized a bit)

	TE_FIREFIELD = 123,		// makes a field of fire.
	// coord (origin)
	// short (radius) (fire is made in a square around origin. -radius, -radius to radius, radius)
	// short (modelindex)
	// byte (count)
	// byte (flags)
	// byte (duration (in seconds) * 10) (will be randomized a bit)
	//
	// to keep network traffic low, this message has associated flags that fit into a byte:
	// LUNA: check enumerator TEFIRE_FLAGS

	TE_PLAYERATTACHMENT = 124, // attaches a TENT to a player (this is a high-priority tent)
	// byte (entity index of player)
	// coord (vertical offset) ( attachment origin.z = player origin.z + vertical offset )
	// short (model index)
	// short (life * 10 ),

	TE_KILLPLAYERATTACHMENTS = 125, // will expire all TENTS attached to a player.
	// byte (entity index of player)

	TE_MULTIGUNSHOT = 126, // much more compact shotgun message
	// This message is used to make a client approximate a 'spray' of gunfire.
	// Any weapon that fires more than one bullet per frame and fires in a bit of a spread is
	// a good candidate for MULTIGUNSHOT use. (shotguns)
	//
	// NOTE: This effect makes the client do traces for each bullet, these client traces ignore
	//		 entities that have studio models.Traces are 4096 long.
	//
	// coord (origin)
	// coord (origin)
	// coord (origin)
	// coord (direction)
	// coord (direction)
	// coord (direction)
	// coord (x noise * 100)
	// coord (y noise * 100)
	// byte (count)
	// byte (bullethole decal texture index)

	TE_USERTRACER = 127, // larger message than the standard tracer, but allows some customization.
	// coord (origin)
	// coord (origin)
	// coord (origin)
	// coord (velocity)
	// coord (velocity)
	// coord (velocity)
	// byte ( life * 10 )
	// byte ( color ) this is an index into an array of color vectors in the engine. (0 - )
	// byte ( length * 10 )
};

// flags of TE_EXPLOSION
EXPORT enum TE_EXPLFLAGS : uint8_t
{
	TE_EXPLFLAG_NONE = 0,			// all flags clear makes default Half-Life explosion
	TE_EXPLFLAG_NOADDITIVE = 1,		// sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
	TE_EXPLFLAG_NODLIGHTS = 2,		// do not render dynamic lights
	TE_EXPLFLAG_NOSOUND = 4,		// do not play client explosion sound
	TE_EXPLFLAG_NOPARTICLES = 8,	// do not draw particles
};

// flags of TE_FIREFIELD
EXPORT enum TEFIRE_FLAGS : uint8_t
{
	TEFIRE_FLAG_ALLFLOAT = 1,	// all sprites will drift upwards as they animate
	TEFIRE_FLAG_SOMEFLOAT = 2,	// some of the sprites will drift upwards. (50% chance)
	TEFIRE_FLAG_LOOP = 4,		// if set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration.
	TEFIRE_FLAG_ALPHA = 8,		// if set, sprite is rendered alpha blended at 50% else, opaque
	TEFIRE_FLAG_PLANAR = 16,	// if set, all fire sprites have same initial Z instead of randomly filling a cube. 
};

// first argument of pfnMessageBegin
EXPORT enum MSG_DEST : uint32_t
{
	MSG_BROADCAST = 0,			// unreliable to all
	MSG_ONE = 1,				// reliable to one (msg_entity)
	MSG_ALL = 2,				// reliable to all
	MSG_INIT = 3,				// write to the init string
	MSG_PVS = 4,				// Ents in PVS of org
	MSG_PAS = 5,				// Ents in PAS of org
	MSG_PVS_R = 6,				// Reliable to PVS
	MSG_PAS_R = 7,				// Reliable to PAS
	MSG_ONE_UNRELIABLE = 8,		// Send to one client, but don't put in reliable stream, put in unreliable datagram ( could be dropped )
	MSG_SPEC = 9,				// Sends to all spectator proxies
};

// contents of a spot in the world
EXPORT enum PT_CONTENTS : int32_t
{
	CONTENTS_EMPTY = -1,
	CONTENTS_SOLID = -2,
	CONTENTS_WATER = -3,
	CONTENTS_SLIME = -4,
	CONTENTS_LAVA = -5,
	CONTENTS_SKY = -6,

	// These additional contents constants are defined in bspfile.h
	CONTENTS_ORIGIN [[deprecated]] = -7,	// removed at csg time
	CONTENTS_CLIP [[deprecated]] = -8,		// changed to contents_solid
	CONTENTS_CURRENT_0 [[deprecated]] = -9,
	CONTENTS_CURRENT_90 [[deprecated]] = -10,
	CONTENTS_CURRENT_180 [[deprecated]] = -11,
	CONTENTS_CURRENT_270 [[deprecated]] = -12,
	CONTENTS_CURRENT_UP [[deprecated]] = -13,
	CONTENTS_CURRENT_DOWN [[deprecated]] = -14,

	CONTENTS_TRANSLUCENT [[deprecated]] = -15,
	// end deprecation

	CONTENTS_LADDER = -16,

	CONTENT_EMPTY = -1,
	CONTENT_SOLID = -2,
	CONTENT_WATER = -3,
	CONTENT_SLIME = -4,
	CONTENT_LAVA = -5,
	CONTENT_SKY = -6,
};

// channels
EXPORT enum SND_CHAN : uint32_t
{
	CHAN_AUTO = 0,
	CHAN_WEAPON = 1,
	CHAN_VOICE = 2,
	CHAN_ITEM = 3,
	CHAN_BODY = 4,
	CHAN_STREAM = 5,				// allocate stream channel from the static or dynamic area
	CHAN_STATIC = 6,				// allocate channel from the static area 
	CHAN_NETWORKVOICE_BASE = 7,		// voice data coming across the network
	CHAN_NETWORKVOICE_END = 500,	// network voice data reserves slots (CHAN_NETWORKVOICE_BASE through CHAN_NETWORKVOICE_END).
};

// attenuation values
EXPORT inline constexpr float ATTN_NONE = 0;
EXPORT inline constexpr float ATTN_NORM = 0.8f;
EXPORT inline constexpr float ATTN_IDLE = 2;
EXPORT inline constexpr float ATTN_STATIC = 1.25f;

// pitch values
EXPORT inline constexpr auto PITCH_NORM = 100;			// non-pitch shifted
EXPORT inline constexpr auto PITCH_LOW = 95;			// other values are possible - 0-255, where 255 is very high
EXPORT inline constexpr auto PITCH_HIGH = 120;

// volume values
EXPORT inline constexpr float VOL_NORM = 1.0;

// plats
EXPORT inline constexpr auto PLAT_LOW_TRIGGER = 1;

// Trains
EXPORT inline constexpr auto SF_TRAIN_WAIT_RETRIGGER = 1;
EXPORT inline constexpr auto SF_TRAIN_PASSABLE = 8;		// Train is not solid -- used to make water trains

// buttons
EXPORT enum BUTTONS : uint32_t
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_SCORE = (1 << 15),   // Used by client.dll for when scoreboard is held down
};

// Break Model Defines
EXPORT enum BREAK_MODEL_TYPE : uint8_t
{
	BREAK_NONE = 0,

	BREAK_TYPEMASK = 0x4F,
	BREAK_GLASS = 0x01,
	BREAK_METAL = 0x02,
	BREAK_FLESH = 0x04,
	BREAK_WOOD = 0x08,

	BREAK_SMOKE = 0x10,
	BREAK_TRANS = 0x20,
	BREAK_CONCRETE = 0x40,
	BREAK_2 = 0x80,
};

EXPORT [[nodiscard]] inline constexpr
BREAK_MODEL_TYPE operator|(BREAK_MODEL_TYPE lhs, BREAK_MODEL_TYPE rhs) noexcept
{
	return static_cast<BREAK_MODEL_TYPE>(
		std::to_underlying(lhs) | std::to_underlying(rhs)
	);
}

// Colliding temp entity sounds
EXPORT enum TE_COL_BOUNCE : uint8_t
{
	BOUNCE_GLASS = BREAK_GLASS,
	BOUNCE_METAL = BREAK_METAL,
	BOUNCE_FLESH = BREAK_FLESH,
	BOUNCE_WOOD = BREAK_WOOD,
	BOUNCE_SHRAP = 0x10,
	BOUNCE_SHELL = 0x20,
	BOUNCE_CONCRETE = BREAK_CONCRETE,
	BOUNCE_SHOTSHELL = 0x80,
};

// Temp entity bounce sound types #SHOULD_DO_ON_FREE Message.ixx requires an update - enum to underlying.
EXPORT enum TE_BOUNCE : uint8_t
{
	TE_BOUNCE_NULL = 0,
	TE_BOUNCE_SHELL = 1,
	TE_BOUNCE_SHOTSHELL = 2,
};

// Rendering constants
EXPORT enum kRenderFn : uint32_t
{
	kRenderNormal,			// src
	kRenderTransColor,		// c*a+dest*(1-a)
	kRenderTransTexture,	// src*a+dest*(1-a)
	kRenderGlow,			// src*a+dest -- No Z buffer checks
	kRenderTransAlpha,		// src*srca+dest*(1-srca)
	kRenderTransAdd,		// src*a+dest
};

EXPORT enum  kRenderFx : uint32_t
{
	kRenderFxNone = 0,
	kRenderFxPulseSlow,
	kRenderFxPulseFast,
	kRenderFxPulseSlowWide,
	kRenderFxPulseFastWide,
	kRenderFxFadeSlow,
	kRenderFxFadeFast,
	kRenderFxSolidSlow,
	kRenderFxSolidFast,
	kRenderFxStrobeSlow,
	kRenderFxStrobeFast,
	kRenderFxStrobeFaster,
	kRenderFxFlickerSlow,
	kRenderFxFlickerFast,
	kRenderFxNoDissipation,
	kRenderFxDistort,			// Distort/scale/translate flicker
	kRenderFxHologram,			// kRenderFxDistort + distance fade
	kRenderFxDeadPlayer,		// kRenderAmt is the player index
	kRenderFxExplode,			// Scale up really big!
	kRenderFxGlowShell,			// Glowing Shell
	kRenderFxClampMinScale,		// Keep this sprite from getting very small (SPRITES only!)
};

EXPORT struct color24
{
	uint8_t r, g, b;
};

EXPORT struct colorVec
{
	uint32_t r, g, b, a;
};

EXPORT struct PackedColorVec
{
	uint16_t r, g, b, a;
};

EXPORT struct plane_t
{
	vec3_t	normal;
	float	dist{};
};

// LUNA: gmsgFade, it's kind of engine-ish.
// The message was never handled by client.dll, but the engine itself.
EXPORT inline constexpr auto FFADE_IN = 0x0000;			// LUNA: Fade into game world.
EXPORT inline constexpr auto FFADE_OUT = 0x0001;			// LUNA: Fade into color screen
EXPORT inline constexpr auto FFADE_MODULATE = 0x0002;	// Modulate (don't blend)
EXPORT inline constexpr auto FFADE_STAYOUT = 0x0004;		// ignores the duration, stays faded out until new ScreenFade message received

#pragma endregion const.h

// There are multiple important constants appears in original util.h
#pragma region util.h

// LUNA: amend through ReHLDS, world.h
EXPORT enum IGNORE_MONSTERS : uint32_t
{
	dont_ignore_monsters = 0,	// normal trace, == MOVE_NORMAL
	ignore_monsters = 1,		// ignore monsters (edicts with flags (FL_MONSTER|FL_FAKECLIENT|FL_CLIENT) set), == MOVE_NOMONSTERS
	missile = 2,				// extra size for monsters, == MOVE_MISSILE

	/*
	* ReHLDS::world.cpp::SV_Move
	* where MOVE_MISSILE == IGNORE_MONSTERS::missile
	if (type == MOVE_MISSILE)
	{
		for (int i = 0; i < 3; i++)
		{
			clip.mins2[i] = -15.0f;
			clip.maxs2[i] = +15.0f;
		}
	}
	*/
};

// LUNA: amend through ReHLDS::world.cpp::SV_ClipToLinks
EXPORT enum IGNORE_GLASS : uint32_t
{
	dont_ignore_glass = 0,
	ignore_glass = 0x100,					// skip entities where (touch->v.rendermode != kRenderNormal && !(touch->v.flags & FL_WORLDBRUSH))
	simplebox [[deprecated]] = 0x200,		// ??? only existed in world.h with no further ref.
};

// LUNA's ext: strongly typed trace argument.
EXPORT enum struct TRACE_FL : uint32_t {};

// LUNA's ext: construct argument for trace series.
EXPORT constexpr auto operator| (IGNORE_MONSTERS lhs, IGNORE_GLASS rhs) noexcept
{
	return
		static_cast<TRACE_FL>(
			std::to_underlying(lhs) | std::to_underlying(rhs)
			);
}
EXPORT constexpr auto operator| (IGNORE_GLASS lhs, IGNORE_MONSTERS rhs) noexcept
{
	return
		static_cast<TRACE_FL>(
			std::to_underlying(lhs) | std::to_underlying(rhs)
			);
}

EXPORT enum HULL_TYPES : uint32_t
{
	/* ReHLDS::pr_cmds.cpp
	vec_t gHullMins[4][3] = {
		{ 0.0f,   0.0f,   0.0f },
		{ -16.0f, -16.0f, -36.0f },
		{ -32.0f, -32.0f, -32.0f },
		{ -16.0f, -16.0f, -18.0f },
	};

	vec_t gHullMaxs[4][3] = {
		{ 0.0f,   0.0f,   0.0f },
		{ 16.0f,  16.0f,  36.0f },
		{ 32.0f,  32.0f,  32.0f },
		{ 16.0f,  16.0f,  18.0f },
	};
	*/

	point_hull = 0, // size == 0
	human_hull = 1,	// size == [-16, 16], tall == [-36, 36], Standed Player
	large_hull = 2,	// size == [-32, 32]
	head_hull = 3,	// size == [-16, 16], tall == [-18, 18], Crouched Player

	// LUNA's extention
	hull_standing = human_hull,
	hull_crouching = head_hull,
};

// for pfnEmitSound
EXPORT enum SOUND_FL : uint32_t
{
	SND_FL_NONE = 0,

	// LUNA: The first five are just for curiosity, they should not be used in real code.

	SND_FL_VOLUME [[deprecated]] = (1 << 0),		// send volume
	SND_FL_ATTENUATION [[deprecated]] = (1 << 1),	// send attenuation
	SND_FL_LARGE_INDEX [[deprecated]] = (1 << 2),	// send sound number as short instead of byte
	SND_FL_PITCH [[deprecated]] = (1 << 3),			// send pitch
	SND_FL_SENTENCE [[deprecated]] = (1 << 4),		// set if sound num is actually a sentence num

	SND_STOP = (1 << 5),			// stop the sound
	SND_CHANGE_VOL = (1 << 6),		// change sound vol
	SND_CHANGE_PITCH = (1 << 7),	// change sound pitch
	SND_SPAWNING = (1 << 8),		// we're spawning, used in some cases for ambients (not sent across network)
};

EXPORT constexpr [[nodiscard]]
SOUND_FL operator| (SOUND_FL lhs, SOUND_FL rhs) noexcept
{
	return static_cast<SOUND_FL>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

// for pfnMessageBegin
// member marked with [[deprecated]] are directly invoking error or mapping to Server::NOp
EXPORT enum ESvcCommands : uint32_t	// LUNA: should be uint8_t but for the sake of ABI...
{
	SVC_BAD [[deprecated]],
	SVC_NOP [[deprecated]],
	SVC_DISCONNECT,
	SVC_EVENT,
	SVC_VERSION,
	SVC_SETVIEW,
	SVC_SOUND,
	SVC_TIME,
	SVC_PRINT,
	SVC_STUFFTEXT,
	SVC_SETANGLE,
	SVC_SERVERINFO,
	SVC_LIGHTSTYLE,
	SVC_UPDATEUSERINFO,
	SVC_DELTADESCRIPTION,
	SVC_CLIENTDATA,
	SVC_STOPSOUND,
	SVC_PINGS,
	SVC_PARTICLE,
	SVC_DAMAGE [[deprecated]],
	SVC_SPAWNSTATIC [[deprecated]],
	SVC_EVENT_RELIABLE,
	SVC_SPAWNBASELINE,
	SVC_TEMPENTITY,
	SVC_SETPAUSE,
	SVC_SIGNONNUM,
	SVC_CENTERPRINT,
	SVC_KILLEDMONSTER [[deprecated]],
	SVC_FOUNDSECRET [[deprecated]],
	SVC_SPAWNSTATICSOUND,
	SVC_INTERMISSION,
	SVC_FINALE,
	SVC_CDTRACK,
	SVC_RESTORE,
	SVC_CUTSCENE,
	SVC_WEAPONANIM,
	SVC_DECALNAME,
	SVC_ROOMTYPE,
	SVC_ADDANGLE,
	SVC_NEWUSERMSG,
	SVC_PACKETENTITIES,
	SVC_DELTAPACKETENTITIES,
	SVC_CHOKE,
	SVC_RESOURCELIST,
	SVC_NEWMOVEVARS,
	SVC_RESOURCEREQUEST,
	SVC_CUSTOMIZATION,
	SVC_CROSSHAIRANGLE,
	SVC_SOUNDFADE,
	SVC_FILETXFERFAILED,
	SVC_HLTV,
	SVC_DIRECTOR,
	SVC_VOICEINIT,
	SVC_VOICEDATA,
	SVC_SENDEXTRAINFO,
	SVC_TIMESCALE,
	SVC_RESOURCELOCATION,
	SVC_SENDCVARVALUE,
	SVC_SENDCVARVALUE2,
	SVC_EXEC,
	SVC_RESERVE60,
	SVC_RESERVE61,
	SVC_RESERVE62,
	SVC_RESERVE63,

	// Double-check with Nagi #INVESTIGATE
	SVC_STARTOFUSERMESSAGES = SVC_RESERVE63 + 1,

	SVC_ENDOFLIST = 255,
};

#pragma endregion util.h

#pragma region progdefs.h

EXPORT inline constexpr size_t MAX_ENT_LEAFS = 48;

EXPORT using qboolean = std::int32_t;
//EXPORT using string_t = std::ptrdiff_t;
EXPORT struct string_t final
{
	constexpr string_t() noexcept = default;
	constexpr string_t(std::ptrdiff_t ofs) noexcept : m_diff{ ofs } {}
	explicit string_t(const char* psz) noexcept;

	constexpr string_t(string_t&&) noexcept = default;
	constexpr string_t(string_t const&) noexcept = default;
	constexpr string_t& operator=(string_t&&) noexcept = default;
	constexpr string_t& operator=(string_t const&) noexcept = default;
	constexpr ~string_t() noexcept = default;

	std::ptrdiff_t m_diff{ 0 };

	constexpr operator std::ptrdiff_t const& () const noexcept { return m_diff; }
	constexpr operator std::ptrdiff_t& () noexcept { return m_diff; }
	operator const char* () const noexcept;
};

EXPORT [[nodiscard]] constexpr auto operator<=>(string_t lhs, string_t rhs) noexcept { return lhs.m_diff <=> rhs.m_diff; };
EXPORT [[nodiscard]] constexpr auto operator==(string_t lhs, string_t rhs) noexcept { return lhs.m_diff == rhs.m_diff; };

#ifndef HLSDK_SKIP_PTR_SIZE_CHECK
static_assert(sizeof(string_t) == 4, "To compatible with GoldSrc Engine, the string_t (a.k.a. std::ptrdiff_t) must be 4 bytes wide.");
#endif

// forward declaration
struct edict_t;
struct entvars_t;
struct link_t;

EXPORT struct entvars_t
{
	string_t	classname{};
	string_t	globalname{};

	vec3_t		origin;
	vec3_t		oldorigin;
	vec3_t		velocity;
	vec3_t		basevelocity;
	vec3_t      clbasevelocity;  // Base velocity that was passed in to server physics so 
	//  client can predict conveyors correctly.  Server zeroes it, so we need to store here, too.
	vec3_t		movedir;

	ang3_t		angles;			// Model angles
	ang3_t		avelocity;		// angle velocity (degrees per second)
	ang3_t		punchangle;		// auto-decaying view angle adjustment
	ang3_t		v_angle;		// Viewing angle (player only)

	// For parametric entities
	vec3_t		endpos;
	vec3_t		startpos;
	float		impacttime{};
	float		starttime{};

	int			fixangle{};		// 0:nothing, 1:force view angles, 2:add avelocity
	float		idealpitch{};
	float		pitch_speed{};
	float		ideal_yaw{};
	float		yaw_speed{};

	int			modelindex{};
	string_t	model{};

	string_t	viewmodel{};		// player's viewmodel
	string_t	weaponmodel{};	// what other players see

	vec3_t		absmin;		// BB max translated to world coord
	vec3_t		absmax;		// BB max translated to world coord
	vec3_t		mins;		// local BB min
	vec3_t		maxs;		// local BB max
	vec3_t		size;		// maxs - mins

	float		ltime{};
	float		nextthink{};

	EVAR_MT		movetype{};
	EVAR_SOLID	solid{};

	int			skin{};
	int			body{};			// sub-model selection for studiomodels
	int 		effects{};

	float		gravity{};		// % of "normal" gravity
	float		friction{};		// inverse elasticity of MOVETYPE_BOUNCE

	int			light_level{};

	int			sequence{};		// animation sequence
	int			gaitsequence{};	// movement animation sequence for player (0 for none)
	float		frame{};			// % playback position in animation sequences (0..255)
	float		animtime{};		// world time when frame was set
	float		framerate{};		// animation playback rate (-8x to 8x)
	uint8_t		controller[4]{};	// bone controller setting (0..255)
	uint8_t		blending[2]{};	// blending amount between sub-sequences (0..255)

	float		scale{};			// sprite rendering scale (0..255)

	kRenderFn	rendermode{};
	float		renderamt{};
	vec3_t		rendercolor;
	kRenderFx	renderfx{};

	float		health{};
	float		frags{};
	uint32_t	weapons{};  // bit mask for available weapons
	float		takedamage{};

	EVAR_DF		deadflag{};
	vec3_t		view_ofs;	// eye position

	int			button{};
	int			impulse{};

	edict_t*	chain{};			// Entity pointer when linked into a linked list
	edict_t*	dmg_inflictor{};
	edict_t*	enemy{};
	edict_t*	aiment{};		// entity pointer when MOVETYPE_FOLLOW
	edict_t*	owner{};
	edict_t*	groundentity{};

	int			spawnflags{};
	int			flags{};

	int			colormap{};		// lowbyte topcolor, highbyte bottomcolor
	int			team{};

	float		max_health{};
	float		teleport_time{};
	float		armortype{};
	float		armorvalue{};
	int			waterlevel{};
	int			watertype{};

	string_t	target{};
	string_t	targetname{};
	string_t	netname{};
	string_t	message{};

	float		dmg_take{};
	float		dmg_save{};
	float		dmg{};
	float		dmgtime{};

	string_t	noise{};
	string_t	noise1{};
	string_t	noise2{};
	string_t	noise3{};

	float		speed{};
	float		air_finished{};
	float		pain_finished{};
	float		radsuit_finished{};

	edict_t*	pContainingEntity{};

	int			playerclass{};
	float		maxspeed{};

	float		fov{};
	int			weaponanim{};

	int			pushmsec{};

	int			bInDuck{};
	int			flTimeStepSound{};
	int			flSwimTime{};
	int			flDuckTime{};
	int			iStepLeft{};
	float		flFallVelocity{};

	EVAR_HGS	gamestate{};

	int			oldbuttons{};

	int			groupinfo{};

	// For mods
	int			iuser1{};
	int			iuser2{};
	int			iuser3{};
	int			iuser4{};
	float		fuser1{};
	float		fuser2{};
	float		fuser3{};
	float		fuser4{};
	vec3_t		vuser1;
	vec3_t		vuser2;
	vec3_t		vuser3;
	vec3_t		vuser4;
	edict_t*	euser1{};
	edict_t*	euser2{};
	edict_t*	euser3{};
	edict_t*	euser4{};
};

EXPORT struct link_t
{
	link_t* prev{}, * next{};
};

EXPORT struct edict_t
{
	qboolean	free{};
	int			serialnumber{};
	link_t		area;				// linked to a division node or leaf

	int			headnode{};			// -1 to use normal leaf check
	int			num_leafs{};
	short		leafnums[MAX_ENT_LEAFS]{};

	float		freetime{};			// sv.time when the object was freed

	void* pvPrivateData{};		// Alloced and freed by engine, used by DLLs

	entvars_t	v;					// C exported fields from progs

	// other fields from progs come immediately after
};

EXPORT struct globalvars_t
{
	float		time{};
	float		frametime{};
	float		force_retouch{};
	string_t	mapname{};
	string_t	startspot{};
	float		deathmatch{};
	float		coop{};
	float		teamplay{};
	float		serverflags{};
	float		found_secrets{};
	vec3_t		v_forward;
	vec3_t		v_up;
	vec3_t		v_right;
	float		trace_allsolid{};
	float		trace_startsolid{};
	float		trace_fraction{};
	vec3_t		trace_endpos;
	vec3_t		trace_plane_normal;
	float		trace_plane_dist{};
	edict_t* trace_ent{};
	float		trace_inopen{};
	float		trace_inwater{};
	int			trace_hitgroup{};
	int			trace_flags{};
	int			msg_entity{};
	int			cdAudioTrack{};
	int			maxClients{};
	int			maxEntities{};
	const char* pStringBase{};

	void* pSaveData{};
	vec3_t		vecLandmarkOffset;
};

EXPORT inline globalvars_t* gpGlobals = nullptr;

#pragma endregion progdefs.h

string_t::string_t(const char* psz) noexcept : m_diff{ psz - gpGlobals->pStringBase } {}
string_t::operator const char* () const noexcept { return gpGlobals->pStringBase + this->m_diff; }

#pragma region cvardef.h

EXPORT enum FCVAR : std::uint32_t
{
	FCVAR_ARCHIVE = (1 << 0),		// set to cause it to be saved to vars.rc
	FCVAR_USERINFO = (1 << 1),		// changes the client's info string
	FCVAR_SERVER = (1 << 2),		// notifies players when changed
	FCVAR_EXTDLL = (1 << 3),		// defined by external DLL
	FCVAR_CLIENTDLL = (1 << 4),		// defined by the client dll
	FCVAR_PROTECTED = (1 << 5),		// It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
	FCVAR_SPONLY = (1 << 6),		// This cvar cannot be changed by clients connected to a multiplayer server.
	FCVAR_PRINTABLEONLY = (1 << 7),	// This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	FCVAR_UNLOGGED = (1 << 8),		// If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
};

EXPORT struct cvar_t
{
	const char* name{};
	const char* string{};
	uint32_t	flags{};
	float		value{};
	cvar_t* next{};
};

#pragma endregion cvardef.h

#pragma region crc.h

EXPORT using CRC32_t = std::uint32_t;

#pragma endregion crc.h

#pragma region custom.h

EXPORT inline constexpr size_t MAX_RESOURCE_LIST = 1280;	// LUNA: only used in engine decompiling project.
EXPORT inline constexpr size_t MAX_QPATH = 64;    // Must match value in quakedefs.h #NO_URGENT LUNA: where is it?

/////////////////
// Customization
// passed to pfnPlayerCustomization
// For automatic downloading.
EXPORT enum resourcetype_t
{
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world,		// Fake type for world, is really t_model
	rt_unk,

	rt_max
};

EXPORT struct resourceinfo_t
{
	struct { int size; } info[7];
};

EXPORT inline constexpr uint32_t RES_FATALIFMISSING = (1 << 0);   // Disconnect if we can't get this file.
EXPORT inline constexpr uint32_t RES_WASMISSING = (1 << 1);   // Do we have the file locally, did we get it ok?
EXPORT inline constexpr uint32_t RES_CUSTOM = (1 << 2);   // Is this resource one that corresponds to another player's customization or is it a server startup resource.
EXPORT inline constexpr uint32_t RES_REQUESTED = (1 << 3);	// Already requested a download of this one
EXPORT inline constexpr uint32_t RES_PRECACHED = (1 << 4);	// Already precached

EXPORT struct resource_t
{
	char			szFileName[MAX_QPATH]; // File name to download/precache.
	resourcetype_t	type;                // t_sound, t_skin, t_model, t_decal.
	int				nIndex;              // For t_decals
	int				nDownloadSize;       // Size in Bytes if this must be downloaded.
	unsigned char	ucFlags;

	// For handling client to client resource propagation
	unsigned char	rgucMD5_hash[16];    // To determine if we already have it.
	unsigned char	playernum;           // Which player index this resource is associated with, if it's a custom resource.

	unsigned char	rguc_reserved[32];	// For future expansion
	resource_t*		pNext;              // Next in chain.
	resource_t*		pPrev;
};

EXPORT struct customization_t
{
	qboolean bInUse;		// Is this customization in use;
	resource_t resource;	// The resource_t for this customization
	qboolean bTranslated;	// Has the raw data been translated into a useable format?  
	//  (e.g., raw decal .wad make into texture_t *)
	int nUserData1;			// Customization specific data
	int nUserData2;			// Customization specific data
	void* pInfo;			// Buffer that holds the data structure that references the data (e.g., the cachewad_t)
	void* pBuffer;			// Buffer that holds the data for the customization (the raw .wad data)
	customization_t* pNext;	// Next in chain
};

EXPORT inline constexpr uint32_t FCUST_FROMHPAK = (1 << 0);
EXPORT inline constexpr uint32_t FCUST_WIPEDATA = (1 << 1);
EXPORT inline constexpr uint32_t FCUST_IGNOREINIT = (1 << 2);

#if 0
EXPORT void	COM_ClearCustomizationList(customization_s* pHead, qboolean bCleanDecals);
EXPORT qboolean	COM_CreateCustomization(customization_s* pListHead, resource_s* pResource, int playernumber, int flags, customization_s** pCustomization, int* nLumps);
EXPORT int COM_SizeofResourceList(resource_s* pList, resourceinfo_s* ri);
#endif

#pragma endregion custom.h

#pragma region com_model.h

EXPORT inline constexpr auto STUDIO_RENDER = 1;
EXPORT inline constexpr auto STUDIO_EVENTS = 2;

EXPORT inline constexpr size_t MAX_CLIENTS = 32;
EXPORT inline constexpr size_t MAX_EDICTS = 900;

EXPORT inline constexpr size_t MAX_MODEL_NAME = 64;
EXPORT inline constexpr size_t MAX_MAP_HULLS = 4;
EXPORT inline constexpr size_t MIPLEVELS = 4;
EXPORT inline constexpr size_t NUM_AMBIENTS = 4;		// automatic ambient sounds
EXPORT inline constexpr size_t MAXLIGHTMAPS = 4;
EXPORT inline constexpr size_t PLANE_ANYZ = 5;

EXPORT inline constexpr auto ALIAS_Z_CLIP_PLANE = 5;

// flags in finalvert_t.flags
EXPORT inline constexpr uint32_t ALIAS_LEFT_CLIP = 0x0001;
EXPORT inline constexpr uint32_t ALIAS_TOP_CLIP = 0x0002;
EXPORT inline constexpr uint32_t ALIAS_RIGHT_CLIP = 0x0004;
EXPORT inline constexpr uint32_t ALIAS_BOTTOM_CLIP = 0x0008;
EXPORT inline constexpr uint32_t ALIAS_Z_CLIP = 0x0010;
EXPORT inline constexpr uint32_t ALIAS_ONSEAM = 0x0020;
EXPORT inline constexpr uint32_t ALIAS_XY_CLIP_MASK = 0x000F;

EXPORT inline constexpr auto ZISCALE = ((float)0x8000);

EXPORT inline constexpr size_t CACHE_SIZE = 32;		// used to align key data structures

EXPORT enum modtype_t
{
	mod_brush,
	mod_sprite,
	mod_alias,
	mod_studio
};

// must match definition in modelgen.h #NO_URGENT LUNA: where is it?
EXPORT enum synctype_t
{
	ST_SYNC = 0,
	ST_RAND
};

EXPORT struct dmodel_t
{
	vec3_t		mins, maxs;
	vec3_t		origin;
	int			headnode[MAX_MAP_HULLS]{};
	int			visleafs{};		// not including the solid leaf 0
	int			firstface{}, numfaces{};
};

// plane_t structure
EXPORT struct mplane_t
{
	vec3_t	normal;			// surface normal
	float	dist{};			// closest appoach to origin
	uint8_t	type{};			// for texture axis selection and fast side tests
	uint8_t	signbits{};		// signx + signy<<1 + signz<<1
	uint8_t	pad[2]{};
};

EXPORT struct mvertex_t
{
	vec3_t		position;
};

EXPORT struct medge_t
{
	unsigned short	v[2];
	unsigned int	cachededgeoffset;
};

EXPORT struct texture_t
{
	char		name[16];
	unsigned	width, height;
	int			anim_total;				// total tenths in sequence ( 0 = no)
	int			anim_min, anim_max;		// time for this frame min <=time< max
	texture_t*	anim_next;				// in the animation sequence
	texture_t*	alternate_anims;		// bmodels in frame 1 use these
	unsigned	offsets[MIPLEVELS];		// four mip maps stored
	unsigned	paloffset;
};

EXPORT struct mtexinfo_t
{
	float		vecs[2][4];		// [s/t] unit vectors in world space. 
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	float		mipadjust;		// ?? mipmap limits for very small surfaces
	texture_t*	texture;
	int			flags;			// sky or slime, no lightmap or 256 subdivision
};

struct msurface_t;

// JAY: Compress this as much as possible
EXPORT struct decal_t
{
	decal_t*	pnext;			// linked list for each surface
	msurface_t*	psurface;		// Surface id for persistence / unlinking
	short		dx;				// Offsets into surface texture (in texture coordinates, so we don't need floats)
	short		dy;
	short		texture;		// Decal texture
	uint8_t		scale;			// Pixel scale
	uint8_t		flags;			// Decal flags

	short		entityIndex;	// Entity this is attached to
};

struct efrag_t;	// check cl_entity.ixx

EXPORT struct mnode_t
{
	// common with leaf
	int			contents;		// 0, to differentiate from leafs
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	mnode_t*	parent;

	// node specific
	mplane_t*	plane;
	mnode_t*	children[2];

	unsigned short	firstsurface;
	unsigned short	numsurfaces;
};

EXPORT struct mleaf_t
{
	// common with node
	int				contents;		// wil be a negative contents number
	int				visframe;		// node needs to be traversed if current

	short			minmaxs[6];		// for bounding box culling

	mnode_t*		parent;

	// leaf specific
	std::byte*		compressed_vis;
	efrag_t*		efrags;

	msurface_t**	firstmarksurface;
	int				nummarksurfaces;
	int				key;			// BSP sequence number for leaf's contents
	uint8_t			ambient_sound_level[NUM_AMBIENTS];
};

struct texture_t;	// #SHOULD_DO_ON_FREE

EXPORT struct surfcache_t
{
	surfcache_t*	next;
	surfcache_t**	owner;
	int				lightadj[4];
	int				dlight;
	int				size;
	unsigned		width;
	unsigned		height;
	float			mipscale;
	texture_t*		texture;
	unsigned char	data[4];
};

EXPORT struct msurface_t
{
	int			visframe{};		// should be drawn when node is crossed

	int			dlightframe{};	// last frame the surface was checked by an animated light
	int			dlightbits{};		// dynamically generated. Indicates if the surface illumination 
	// is modified by an animated light.

	mplane_t* plane{};			// pointer to shared plane			
	int			flags{};			// see SURF_ #defines

	int			firstedge{};	// look up in model->surfedges[], negative numbers
	int			numedges{};	// are backwards edges

	// surface generation data
	surfcache_t* cachespots[MIPLEVELS]{};

	short		texturemins[2]{}; // smallest s/t position on the surface.
	short		extents[2]{};		// ?? s/t texture size, 1..256 for all non-sky surfaces

	mtexinfo_t* texinfo{};

	// lighting info
	uint8_t		styles[MAXLIGHTMAPS]{}; // index into d_lightstylevalue[] for animated lights 
	// no one surface can be effected by more than 4 
	// animated lights.
	color24* samples{};

	decal_t* pdecals{};
};

EXPORT struct dclipnode_t
{
	int			planenum;
	short		children[2];	// negative numbers are contents
};

EXPORT struct hull_t
{
	dclipnode_t* clipnodes{};
	mplane_t*	planes{};
	int			firstclipnode{};
	int			lastclipnode{};
	vec3_t		clip_mins;
	vec3_t		clip_maxs;
};

EXPORT struct cache_user_t
{
	void* data;
};

EXPORT struct model_t
{
	char		name[MAX_MODEL_NAME]{};
	qboolean	needload{};		// bmodels and sprites don't cache normally

	modtype_t	type{};
	uint32_t	numframes{};
	synctype_t	synctype{};

	int			flags{};

	//
	// volume occupied by the model
	//		
	vec3_t		mins, maxs;
	float		radius{};

	//
	// brush model
	//
	int			firstmodelsurface{}, nummodelsurfaces{};

	uint32_t	numsubmodels{};
	dmodel_t*	submodels{};

	uint32_t	numplanes{};
	mplane_t*	planes{};

	uint32_t	numleafs{};		// number of visible leafs, not counting 0
	mleaf_t*	leafs{};

	uint32_t	numvertexes{};
	mvertex_t*	vertexes{};

	uint32_t	numedges{};
	medge_t*	edges{};

	uint32_t	numnodes{};
	mnode_t*	nodes{};

	uint32_t	numtexinfo{};
	mtexinfo_t*	texinfo{};

	uint32_t	numsurfaces{};
	msurface_t*	surfaces{};

	uint32_t	numsurfedges{};
	int*		surfedges{};

	uint32_t	numclipnodes{};
	dclipnode_t* clipnodes{};

	uint32_t	nummarksurfaces{};
	msurface_t** marksurfaces{};

	hull_t		hulls[MAX_MAP_HULLS];

	uint32_t	numtextures{};
	texture_t** textures{};

	std::byte*	visdata{};

	color24*	lightdata{};

	char*		entities{};

	//
	// additional model data
	//
	cache_user_t	cache{};		// only access through Mod_Extradata

};

EXPORT struct alight_t
{
	int			ambientlight{};	// clip at 128
	int			shadelight{};		// clip at 192 - ambientlight
	vec3_t		color;
	float*		plightvec{};
};

EXPORT struct auxvert_t
{
	float	fv[3];		// viewspace x, y
};

EXPORT inline constexpr size_t MAX_INFO_STRING = 256;
EXPORT inline constexpr size_t MAX_SCOREBOARDNAME = 32;

EXPORT struct player_info_t
{
	// User id on server
	int		userid{};

	// User info string
	char	userinfo[MAX_INFO_STRING]{};

	// Name
	char	name[MAX_SCOREBOARDNAME]{};

	// Spectator or not, unused
	int		spectator{};

	int		ping{};
	int		packet_loss{};

	// skin information
	char	model[MAX_QPATH]{};
	int		topcolor{};
	int		bottomcolor{};

	// last frame rendered
	int		renderframe{};

	// Gait frame estimation
	int		gaitsequence{};
	float	gaitframe{};
	float	gaityaw{};
	vec3_t	prevgaitorigin;

	customization_t customdata{};
};

#pragma endregion com_model.h

#pragma region studio.h

/*
==============================================================================

STUDIO MODELS

Studio models are position independent, so the cache manager can move them.
==============================================================================
*/

EXPORT inline constexpr size_t MAXSTUDIOTRIANGLES = 20000;	// TODO: tune this
EXPORT inline constexpr size_t MAXSTUDIOVERTS = 2048;		// TODO: tune this
EXPORT inline constexpr size_t MAXSTUDIOSEQUENCES = 256;	// total animation sequences
EXPORT inline constexpr size_t MAXSTUDIOSKINS = 100;		// total textures
EXPORT inline constexpr size_t MAXSTUDIOSRCBONES = 512;		// bones allowed at source movement
EXPORT inline constexpr size_t MAXSTUDIOBONES = 128;		// total bones actually used
EXPORT inline constexpr size_t MAXSTUDIOMODELS = 32;		// sub-models per model
EXPORT inline constexpr size_t MAXSTUDIOBODYPARTS = 32;
EXPORT inline constexpr size_t MAXSTUDIOGROUPS = 16;
EXPORT inline constexpr size_t MAXSTUDIOANIMATIONS = 512;	// per sequence
EXPORT inline constexpr size_t MAXSTUDIOMESHES = 256;
EXPORT inline constexpr size_t MAXSTUDIOEVENTS = 1024;
EXPORT inline constexpr size_t MAXSTUDIOPIVOTS = 256;
EXPORT inline constexpr size_t MAXSTUDIOCONTROLLERS = 8;

EXPORT struct studiohdr_t
{
	int					id{};
	int					version{};

	char				name[64]{};
	int					length{};

	vec3_t				eyeposition;	// ideal eye position
	vec3_t				min;			// ideal movement hull size
	vec3_t				max;

	vec3_t				bbmin;			// clipping bounding box
	vec3_t				bbmax;

	uint32_t			flags{};

	uint32_t			numbones{};			// bones
	ptrdiff_t			boneindex{};

	uint32_t			numbonecontrollers{};		// bone controllers
	ptrdiff_t			bonecontrollerindex{};

	uint32_t			numhitboxes{};			// complex bounding boxes
	ptrdiff_t			hitboxindex{};

	uint32_t			numseq{};				// animation sequences
	ptrdiff_t			seqindex{};

	uint32_t			numseqgroups{};		// demand loaded sequences
	ptrdiff_t			seqgroupindex{};

	uint32_t			numtextures{};		// raw textures
	ptrdiff_t			textureindex{};
	ptrdiff_t			texturedataindex{};

	uint32_t			numskinref{};			// replaceable textures
	uint32_t			numskinfamilies{};
	ptrdiff_t			skinindex{};

	uint32_t			numbodyparts{};
	ptrdiff_t			bodypartindex{};

	uint32_t			numattachments{};		// queryable attachable points
	ptrdiff_t			attachmentindex{};

	// HLAM: This seems to be obsolete. Probably replaced by events that reference external sounds?
	int32_t				soundtable{};
	int32_t				soundindex{};
	int32_t				soundgroups{};
	int32_t				soundgroupindex{};

	uint32_t			numtransitions{};		// animation node to animation node transition graph
	ptrdiff_t			transitionindex{};
};

// header for demand loaded sequence group data
EXPORT struct studioseqhdr_t
{
	int					id;
	int					version;

	char				name[64];
	int					length;
};

// bones
EXPORT struct mstudiobone_t
{
	char				name[32];	// bone name for symbolic links
	int		 			parent;		// parent bone
	int					flags;		// ??
	int					bonecontroller[6];	// bone controller index, -1 == none
	float				value[6];	// default DoF values
	float				scale[6];   // scale for delta DoF values
};

// bone controllers
EXPORT struct mstudiobonecontroller_t
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					index;	// 0-3 user set controller, 4 mouth
};

// intersection boxes
EXPORT struct mstudiobbox_t
{
	int					bone{};
	int					group{};			// intersection group
	vec3_t				bbmin;		// bounding box
	vec3_t				bbmax;
};

// demand loaded sequence groups
EXPORT struct mstudioseqgroup_t
{
	char				label[32]{};	// textual name
	char				name[64]{};	// file name
	cache_user_t		cache{};		// cache index pointer
	int					data{};		// hack for group 0
};

// sequence descriptions
EXPORT struct mstudioseqdesc_t
{
	char				label[32]{};	// sequence label

	float				fps{};		// frames per second	
	int					flags{};		// looping/non-looping flags

	int					activity{};
	int					actweight{};

	uint32_t			numevents{};
	ptrdiff_t			eventindex{};

	uint32_t			numframes{};	// number of frames per sequence

	uint32_t			numpivots{};	// number of foot pivots
	int					pivotindex{};

	int					motiontype{};
	int					motionbone{};
	vec3_t				linearmovement;
	int					automoveposindex{};
	int					automoveangleindex{};

	vec3_t				bbmin;		// per sequence bounding box
	vec3_t				bbmax;

	int					numblends{};
	int					animindex{};		// mstudioanim_t pointer relative to start of sequence group data
	// [blend][bone][X, Y, Z, XR, YR, ZR]

	int					blendtype[2]{};	// X, Y, Z, XR, YR, ZR
	float				blendstart[2]{};	// starting value
	float				blendend[2]{};	// ending value
	int					blendparent{};

	int					seqgroup{};		// sequence group for demand loading

	int					entrynode{};		// transition node at entry
	int					exitnode{};		// transition node at exit
	int					nodeflags{};		// transition rules

	int					nextseq{};		// auto advancing sequences
};

// events
EXPORT struct mstudioevent_t
{
	int 				frame;
	int					event;
	int					type;
	char				options[64];
};

// pivots
EXPORT struct mstudiopivot_t
{
	vec3_t				org;	// pivot point
	int					start{};
	int					end{};
};

// attachment
EXPORT struct mstudioattachment_t
{
	char				name[32]{};
	int					type{};
	int					bone{};
	vec3_t				org;	// attachment point
	vec3_t				vectors[3];
};

EXPORT struct mstudioanim_t
{
	unsigned short	offset[6];
};

// animation frames
EXPORT union mstudioanimvalue_t
{
	struct {
		uint8_t	valid;
		uint8_t	total;
	} num;
	uint16_t	value;
};

// body part index
EXPORT struct mstudiobodyparts_t
{
	char				name[64];
	uint32_t			nummodels;
	int32_t				base;
	ptrdiff_t			modelindex; // index into models array
};

// skin info
EXPORT struct mstudiotexture_t
{
	char					name[64];
	int						flags;
	int						width;
	int						height;
	int						index;
};


// skin families
// short	index[skinfamilies][skinref]

// studio models
EXPORT struct mstudiomodel_t
{
	char				name[64];

	int32_t				type;

	float				boundingradius;

	uint32_t			nummesh;
	ptrdiff_t			meshindex;

	uint32_t			numverts;		// number of unique vertices
	ptrdiff_t			vertinfoindex;	// vertex bone info
	ptrdiff_t			vertindex;		// vertex vec3_t
	uint32_t			numnorms;		// number of unique surface normals
	ptrdiff_t			norminfoindex;	// normal bone info
	ptrdiff_t			normindex;		// normal vec3_t

	uint32_t			numgroups;		// deformation groups
	ptrdiff_t			groupindex;
};


// vec3_t	boundingbox[model][bone][2];	// complex intersection info


// meshes
EXPORT struct mstudiomesh_t
{
	int					numtris;
	int					triindex;
	int					skinref;
	int					numnorms;		// per mesh normals
	int					normindex;		// normal vec3_t
};

// triangles
#if 0
EXPORT struct mstudiotrivert_t
{
	short				vertindex;		// index into vertex array
	short				normindex;		// index into normal array
	short				s, t;			// s,t position on skin
};
#endif

// lighting options
EXPORT inline constexpr uint32_t STUDIO_NF_FLATSHADE = 0x0001;
EXPORT inline constexpr uint32_t STUDIO_NF_CHROME = 0x0002;
EXPORT inline constexpr uint32_t STUDIO_NF_FULLBRIGHT = 0x0004;

// motion flags
EXPORT inline constexpr uint32_t STUDIO_X = 0x0001;
EXPORT inline constexpr uint32_t STUDIO_Y = 0x0002;
EXPORT inline constexpr uint32_t STUDIO_Z = 0x0004;
EXPORT inline constexpr uint32_t STUDIO_XR = 0x0008;
EXPORT inline constexpr uint32_t STUDIO_YR = 0x0010;
EXPORT inline constexpr uint32_t STUDIO_ZR = 0x0020;
EXPORT inline constexpr uint32_t STUDIO_LX = 0x0040;
EXPORT inline constexpr uint32_t STUDIO_LY = 0x0080;
EXPORT inline constexpr uint32_t STUDIO_LZ = 0x0100;
EXPORT inline constexpr uint32_t STUDIO_AX = 0x0200;
EXPORT inline constexpr uint32_t STUDIO_AY = 0x0400;
EXPORT inline constexpr uint32_t STUDIO_AZ = 0x0800;
EXPORT inline constexpr uint32_t STUDIO_AXR = 0x1000;
EXPORT inline constexpr uint32_t STUDIO_AYR = 0x2000;
EXPORT inline constexpr uint32_t STUDIO_AZR = 0x4000;
EXPORT inline constexpr uint32_t STUDIO_TYPES = 0x7FFF;
EXPORT inline constexpr uint32_t STUDIO_RLOOP = 0x8000;	// controller that wraps shortest distance

// sequence flags
EXPORT inline constexpr uint32_t STUDIO_LOOPING = 0x0001;

// bone flags
EXPORT inline constexpr uint32_t STUDIO_HAS_NORMALS = 0x0001;
EXPORT inline constexpr uint32_t STUDIO_HAS_VERTICES = 0x0002;
EXPORT inline constexpr uint32_t STUDIO_HAS_BBOX = 0x0004;
EXPORT inline constexpr uint32_t STUDIO_HAS_CHROME = 0x0008;	// if any of the textures have chrome on them

EXPORT inline constexpr auto RAD_TO_STUDIO = (32768.0 / std::numbers::pi);
EXPORT inline constexpr auto STUDIO_TO_RAD = (std::numbers::pi / 32768.0);

static_assert(RAD_TO_STUDIO != 0 and STUDIO_TO_RAD != 0);

#pragma endregion studio.h

#pragma region weaponinfo.h

EXPORT struct weapon_data_t
{
	int			m_iId;
	int			m_iClip;

	float		m_flNextPrimaryAttack;
	float		m_flNextSecondaryAttack;
	float		m_flTimeWeaponIdle;

	int			m_fInReload;
	int			m_fInSpecialReload;
	float		m_flNextReload;
	float		m_flPumpTime;
	float		m_fReloadTime;

	float		m_fAimedDamage;
	float		m_fNextAimBonus;
	int			m_fInZoom;
	int			m_iWeaponState;

	int			iuser1;
	int			iuser2;
	int			iuser3;
	int			iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
};
#pragma endregion weaponinfo.h

#pragma region usercmd.h

EXPORT struct usercmd_t
{
	short	lerp_msec{};		// Interpolation time on client
	uint8_t	msec{};				// Duration in ms of command
	vec3_t	viewangles;			// Command view angles.

	// intended velocities
	float	forwardmove{};		// Forward velocity.
	float	sidemove{};			// Sideways velocity.
	float	upmove{};			// Upward velocity.
	uint8_t	lightlevel{};		// Light level at spot where we are standing.
	uint16_t buttons{};			// Attack buttons
	uint8_t	impulse{};			// Impulse command issued.
	uint8_t	weaponselect{};		// Current weapon id

	// Experimental player impact stuff.
	int		impact_index{};
	vec3_t	impact_position;
};
#pragma endregion usercmd.h

#pragma region pm_info.h
EXPORT inline constexpr size_t MAX_PHYSINFO_STRING = 256;
#pragma endregion pm_info.h

#pragma region pmtrace.h

// PM_PlayerTrace results.

EXPORT struct pmplane_t
{
	vec3_t	normal;
	float	dist{};
};

EXPORT struct pmtrace_t
{
	qboolean	allsolid{};			// if true, plane is not valid
	qboolean	startsolid{};		// if true, the initial point was in a solid area
	qboolean	inopen{}, inwater{};// End point is in empty space or in water
	float		fraction{};			// time completed, 1.0 = didn't hit anything
	vec3_t		endpos;				// final position
	pmplane_t	plane;				// surface normal at impact
	int			ent{};				// entity at impact
	vec3_t      deltavelocity;		// Change in player's velocity caused by impact. 

	// Only run on server.
	int         hitgroup{};
};

EXPORT struct trace_t	// LUNA: in const.h originally.
{
	qboolean	allsolid{};		// if true, plane is not valid
	qboolean	startsolid{};		// if true, the initial point was in a solid area
	qboolean	inopen{}, inwater{};
	float		fraction{};		// time completed, 1.0 = didn't hit anything
	vec3_t		endpos;			// final position
	plane_t		plane;			// surface normal at impact
	edict_t*	ent{};			// entity the surface is on
	int			hitgroup{};		// 0 == generic, non zero is specific body part
};

#pragma endregion pmtrace.h

#pragma region pm_movevars.h

// movevars_t                  // Physics variables.
EXPORT struct movevars_t
{
	float	gravity;           // Gravity for map
	float	stopspeed;         // Deceleration when not moving
	float	maxspeed;          // Max allowed speed
	float	spectatormaxspeed;
	float	accelerate;        // Acceleration factor
	float	airaccelerate;     // Same for when in open air
	float	wateraccelerate;   // Same for when in water
	float	friction;
	float   edgefriction;	   // Extra friction near dropofs 
	float	waterfriction;     // Less in water
	float	entgravity;        // 1.0
	float   bounce;            // Wall bounce value. 1.0
	float   stepsize;          // sv_stepsize;
	float   maxvelocity;       // maximum server velocity.
	float	zmax;			   // Max z-buffer range (for GL)
	float	waveHeight;		   // Water wave height (for GL)
	qboolean footsteps;        // Play footstep sounds
	char	skyName[32];	   // Name of the sky map
	float	rollangle;
	float	rollspeed;
	float	skycolor_r;			// Sky color
	float	skycolor_g;			// 
	float	skycolor_b;			//
	float	skyvec_x;			// Sky vector
	float	skyvec_y;			// 
	float	skyvec_z;			// 
};
#pragma endregion pm_movevars.h

#pragma region event_flags.h

EXPORT enum EFEV : uint32_t
{
	// Skip local host for event send.
	FEV_NOTHOST = (1 << 0),

	// Send the event reliably.  You must specify the origin and angles and use
	// PLAYBACK_EVENT_FULL for this to work correctly on the server for anything
	// that depends on the event origin/angles.  I.e., the origin/angles are not
	// taken from the invoking edict for reliable events.
	FEV_RELIABLE = (1 << 1),

	// Don't restrict to PAS/PVS, send this event to _everybody_ on the server ( useful for stopping CHAN_STATIC
	//  sounds started by client event when client is not in PVS anymore ( hwguy in TFC e.g. ).
	FEV_GLOBAL = (1 << 2),

	// If this client already has one of these events in its queue, just update the event instead of sending it as a duplicate
	FEV_UPDATE = (1 << 3),

	// Only send to entity specified as the invoker
	FEV_HOSTONLY = (1 << 4),

	// Only send if the event was created on the server.
	FEV_SERVER = (1 << 5),

	// Only issue event client side ( from shared code )
	FEV_CLIENT = (1 << 6),
};

EXPORT inline constexpr EFEV operator|(EFEV lhs, EFEV rhs) noexcept
{
	return (EFEV)(std::to_underlying(lhs) | std::to_underlying(rhs));
}

#pragma endregion event_flags.h

#pragma region pm_defs.h

EXPORT inline constexpr size_t MAX_PHYSENTS = 600;					// Must have room for all entities in the world.
EXPORT inline constexpr size_t MAX_MOVEENTS = 64;
EXPORT inline constexpr size_t MAX_CLIP_PLANES = 5;

EXPORT inline constexpr uint32_t PM_NORMAL = 0x00000000;
EXPORT inline constexpr uint32_t PM_STUDIO_IGNORE = 0x00000001;		// Skip studio models
EXPORT inline constexpr uint32_t PM_STUDIO_BOX = 0x00000002;		// Use boxes for non-complex studio models (even in traceline)
EXPORT inline constexpr uint32_t PM_GLASS_IGNORE = 0x00000004;		// Ignore entities with non-normal rendermode
EXPORT inline constexpr uint32_t PM_WORLD_ONLY = 0x00000008;		// Only trace against the world

// Values for flags parameter of PM_TraceLine
EXPORT inline constexpr auto PM_TRACELINE_PHYSENTSONLY = 0;
EXPORT inline constexpr auto PM_TRACELINE_ANYVISIBLE = 1;

// physent_t
EXPORT struct physent_t
{
	char			name[32]{};			// Name of model, or "player" or "world".
	int				player{};
	vec3_t			origin;				// Model's origin in world coordinates.
	model_t*		model{};			// only for bsp models
	model_t*		studiomodel{};		// SOLID_BBOX, but studio clip intersections.
	vec3_t			mins, maxs;			// only for non-bsp models
	int				info{};				// For client or server to use to identify (index into edicts or cl_entities)
	vec3_t			angles;				// rotated entities need this info for hull testing to work.

	int				solid{};			// Triggers and func_door type WATER brushes are SOLID_NOT
	int				skin{};				// BSP Contents for such things like fun_door water brushes.
	int				rendermode{};		// So we can ignore glass

	// Complex collision detection.
	float			frame{};
	int				sequence{};
	uint8_t			controller[4]{};
	uint8_t			blending[2]{};

	int				movetype{};
	int				takedamage{};
	int				blooddecal{};
	int				team{};
	int				classnumber{};

	// For mods
	int				iuser1{};
	int				iuser2{};
	int				iuser3{};
	int				iuser4{};
	float			fuser1{};
	float			fuser2{};
	float			fuser3{};
	float			fuser4{};
	vec3_t			vuser1;
	vec3_t			vuser2;
	vec3_t			vuser3;
	vec3_t			vuser4;
};

EXPORT struct playermove_t
{
	int				player_index{};  // So we don't try to run the PM_CheckStuck nudging too quickly.
	qboolean		server{};        // For debugging, are we running physics code on server side?

	qboolean		multiplayer{};   // 1 == multiplayer server
	float			time{};          // realtime on host, for reckoning duck timing
	float			frametime{};	   // Duration of this frame

	vec3_t			forward, right, up; // Vectors for angles
	// player state
	vec3_t			origin;        // Movement origin.
	vec3_t			angles;        // Movement view angles.
	vec3_t			oldangles;     // Angles before movement view angles were looked at.
	vec3_t			velocity;      // Current movement direction.
	vec3_t			movedir;       // For waterjumping, a forced forward velocity so we can fly over lip of ledge.
	vec3_t			basevelocity;  // Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
	vec3_t			view_ofs;      // Our eye position.
	float			flDuckTime{};    // Time we started duck
	qboolean		bInDuck{};       // In process of ducking or ducked already?

	// For walking/falling
	int				flTimeStepSound{};  // Next time we can play a step sound
	int				iStepLeft{};

	float			flFallVelocity{};
	vec3_t			punchangle;

	float			flSwimTime{};

	float			flNextPrimaryAttack{};

	int				effects{};		// MUZZLE FLASH, e.g.

	int				flags{};         // FL_ONGROUND, FL_DUCKING, etc.
	int				usehull{};       // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	float			gravity{};       // Our current gravity and friction.
	float			friction{};
	int				oldbuttons{};    // Buttons last usercmd
	float			waterjumptime{}; // Amount of time left in jumping out of water cycle.
	qboolean		dead{};          // Are we a dead player?
	int				deadflag{};
	int				spectator{};     // Should we use spectator physics model?
	int				movetype{};      // Our movement type, NOCLIP, WALK, FLY

	int				onground{};
	int				waterlevel{};
	int				watertype{};
	int				oldwaterlevel{};

	char			sztexturename[256]{};
	char			chtexturetype{};

	float			maxspeed{};
	float			clientmaxspeed{}; // Player specific maxspeed

	// For mods
	int				iuser1{};
	int				iuser2{};
	int				iuser3{};
	int				iuser4{};
	float			fuser1{};
	float			fuser2{};
	float			fuser3{};
	float			fuser4{};
	vec3_t			vuser1;
	vec3_t			vuser2;
	vec3_t			vuser3;
	vec3_t			vuser4;
	// world state
	// Number of entities to clip against.
	uint32_t		numphysent{};
	physent_t		physents[MAX_PHYSENTS];
	// Number of momvement entities (ladders)
	uint32_t		nummoveent{};
	// just a list of ladders
	physent_t		moveents[MAX_MOVEENTS];

	// All things being rendered, for tracing against things you don't actually collide with
	uint32_t		numvisent{};
	physent_t		visents[MAX_PHYSENTS];

	// input to run through physics.
	usercmd_t		cmd;

	// Trace results for objects we collided with.
	uint32_t		numtouch{};
	pmtrace_t		touchindex[MAX_PHYSENTS];

	char			physinfo[MAX_PHYSINFO_STRING]{}; // Physics info string

	movevars_t*		movevars{};
	vec3_t player_mins[4];
	vec3_t player_maxs[4];

	// Common functions
	const char*		(*PM_Info_ValueForKey) (const char* s, const char* key){};
	void			(*PM_Particle)(float* origin, int color, float life, int zpos, int zvel){};
	int				(*PM_TestPlayerPosition) (float* pos, pmtrace_t* ptrace){};
	void			(*Con_NPrintf)(int idx, char* fmt, ...){};
	void			(*Con_DPrintf)(char* fmt, ...){};
	void			(*Con_Printf)(char* fmt, ...){};
	double			(*Sys_FloatTime)(void){};
	void			(*PM_StuckTouch)(int hitent, pmtrace_t* ptraceresult){};
	int				(*PM_PointContents) (float* p, int* truecontents /*filled in if this is non-null*/){};
	int				(*PM_TruePointContents) (float* p){};
	int				(*PM_HullPointContents) (hull_t* hull, int num, float* p){};
	pmtrace_t		(*PM_PlayerTrace) (float* start, float* end, int traceFlags, int ignore_pe){};
	pmtrace_t*		(*PM_TraceLine)(float* start, float* end, int flags, int usehulll, int ignore_pe){};
	long			(*RandomLong)(long lLow, long lHigh){};
	float			(*RandomFloat)(float flLow, float flHigh){};
	int				(*PM_GetModelType)(model_t* mod){};
	void			(*PM_GetModelBounds)(model_t* mod, float* mins, float* maxs){};
	hull_t*			(*PM_HullForBsp)(physent_t* pe, float* offset){};
	float			(*PM_TraceModel)(physent_t* pEnt, float* start, float* end, trace_t* trace){};
	int				(*COM_FileSize)(char* filename){};
	std::byte*		(*COM_LoadFile) (char* path, int usehunk, int* pLength){};
	void			(*COM_FreeFile) (void* buffer){};
	char*			(*memfgets)(std::byte* pMemFile, int fileSize, int* pFilePos, char* pBuffer, int bufferSize){};

	// Functions
	// Run functions for this frame?
	qboolean		runfuncs{};
	void			(*PM_PlaySound) (int channel, const char* sample, float volume, float attenuation, int fFlags, int pitch){};
	const char*		(*PM_TraceTexture) (int ground, float* vstart, float* vend){};
	void			(*PM_PlaybackEventFull) (EFEV flags, int clientindex, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2){};

	pmtrace_t		(*PM_PlayerTraceEx) (float* start, float* end, int traceFlags, int (*pfnIgnore)(physent_t* pe)){};
	int				(*PM_TestPlayerPositionEx) (float* pos, pmtrace_t* ptrace, int (*pfnIgnore)(physent_t* pe)){};
	pmtrace_t*		(*PM_TraceLineEx)(float* start, float* end, int flags, int usehulll, int (*pfnIgnore)(physent_t* pe)){};
};

#pragma endregion pm_defs.h

#pragma region entity_state.h

// For entityType below
EXPORT inline constexpr uint32_t ENTITY_NORMAL = (1 << 0);
EXPORT inline constexpr uint32_t ENTITY_BEAM = (1 << 1);

// Entity state is used for the baseline and for delta compression of a packet of 
//  entities that is sent to a client.
EXPORT struct entity_state_t
{
	// Fields which are filled in by routines outside of delta compression
	int			entityType{};
	// Index into cl_entities array for this entity.
	int			number{};
	float		msg_time{};

	// Message number last time the player/entity state was updated.
	int			messagenum{};

	// Fields which can be transitted and reconstructed over the network stream
	vec3_t		origin;
	vec3_t		angles;

	int			modelindex{};
	int			sequence{};
	float		frame{};
	int			colormap{};
	short		skin{};
	short		solid{};
	int			effects{};
	float		scale{};

	uint8_t		eflags{};

	// Render information
	kRenderFn	rendermode{};
	int			renderamt{};
	color24		rendercolor{};
	kRenderFx	renderfx{};

	int			movetype{};
	float		animtime{};
	float		framerate{};
	int			body{};
	uint8_t		controller[4]{};
	uint8_t		blending[4]{};
	vec3_t		velocity;

	// Send bbox down to client for use during prediction.
	vec3_t		mins;
	vec3_t		maxs;

	int			aiment{};
	// If owned by a player, the index of that player ( for projectiles ).
	int			owner{};

	// Friction, for prediction.
	float		friction{};
	// Gravity multiplier
	float		gravity{};

	// PLAYER SPECIFIC
	int			team{};
	int			playerclass{};
	int			health{};
	qboolean	spectator{};
	int         weaponmodel{};
	int			gaitsequence{};
	// If standing on conveyor, e.g.
	vec3_t		basevelocity;
	// Use the crouched hull, or the regular player hull.
	int			usehull{};
	// Latched buttons last time state updated.
	int			oldbuttons{};
	// -1 = in air, else pmove entity number
	int			onground{};
	int			iStepLeft{};
	// How fast we are falling
	float		flFallVelocity{};

	float		fov{};
	int			weaponanim{};

	// Parametric movement overrides
	vec3_t		startpos;
	vec3_t		endpos;
	float		impacttime{};
	float		starttime{};

	// For mods
	int			iuser1{};
	int			iuser2{};
	int			iuser3{};
	int			iuser4{};
	float		fuser1{};
	float		fuser2{};
	float		fuser3{};
	float		fuser4{};
	vec3_t		vuser1;
	vec3_t		vuser2;
	vec3_t		vuser3;
	vec3_t		vuser4;
};

EXPORT struct clientdata_t
{
	vec3_t				origin;
	vec3_t				velocity;

	int					viewmodel{};
	vec3_t				punchangle;
	int					flags{};
	int					waterlevel{};
	int					watertype{};
	vec3_t				view_ofs;
	float				health{};

	int					bInDuck{};

	int					weapons{}; // remove?

	int					flTimeStepSound{};
	int					flDuckTime{};
	int					flSwimTime{};
	int					waterjumptime{};

	float				maxspeed{};

	float				fov{};
	int					weaponanim{};

	int					m_iId{};
	int					ammo_shells{};
	int					ammo_nails{};
	int					ammo_cells{};
	int					ammo_rockets{};
	float				m_flNextAttack{};

	int					tfstate{};

	int					pushmsec{};

	int					deadflag{};

	char				physinfo[MAX_PHYSINFO_STRING]{};

	// For mods
	int					iuser1{};
	int					iuser2{};
	int					iuser3{};
	int					iuser4{};
	float				fuser1{};
	float				fuser2{};
	float				fuser3{};
	float				fuser4{};
	vec3_t				vuser1;
	vec3_t				vuser2;
	vec3_t				vuser3;
	vec3_t				vuser4;
};

EXPORT struct local_state_t
{
	entity_state_t	playerstate;
	clientdata_t	client;
	weapon_data_t	weapondata[32]{};
};

#pragma endregion entity_state.h

#pragma region Sequence.h

//---------------------------------------------------------------------------
// client_textmessage_t
//---------------------------------------------------------------------------
EXPORT struct client_textmessage_t
{
	int		effect;
	uint8_t	r1, g1, b1, a1;		// 2 colors for effects
	uint8_t	r2, g2, b2, a2;
	float	x;
	float	y;
	float	fadein;
	float	fadeout;
	float	holdtime;
	float	fxtime;
	const char* pName;
	const char* pMessage;
};

//--------------------------------------------------------------------------
// sequenceDefaultBits_e
//	
// Enumerated list of possible modifiers for a command.  This enumeration
// is used in a bitarray controlling what modifiers are specified for a command.
//---------------------------------------------------------------------------
EXPORT enum sequenceModifierBits
{
	SEQUENCE_MODIFIER_EFFECT_BIT = (1 << 1),
	SEQUENCE_MODIFIER_POSITION_BIT = (1 << 2),
	SEQUENCE_MODIFIER_COLOR_BIT = (1 << 3),
	SEQUENCE_MODIFIER_COLOR2_BIT = (1 << 4),
	SEQUENCE_MODIFIER_FADEIN_BIT = (1 << 5),
	SEQUENCE_MODIFIER_FADEOUT_BIT = (1 << 6),
	SEQUENCE_MODIFIER_HOLDTIME_BIT = (1 << 7),
	SEQUENCE_MODIFIER_FXTIME_BIT = (1 << 8),
	SEQUENCE_MODIFIER_SPEAKER_BIT = (1 << 9),
	SEQUENCE_MODIFIER_LISTENER_BIT = (1 << 10),
	SEQUENCE_MODIFIER_TEXTCHANNEL_BIT = (1 << 11),
};

EXPORT using sequenceModifierBits_e = sequenceModifierBits;


//---------------------------------------------------------------------------
// sequenceCommandEnum_e
// 
// Enumerated sequence command types.
//---------------------------------------------------------------------------
EXPORT enum sequenceCommandEnum_e
{
	SEQUENCE_COMMAND_ERROR = -1,
	SEQUENCE_COMMAND_PAUSE = 0,
	SEQUENCE_COMMAND_FIRETARGETS,
	SEQUENCE_COMMAND_KILLTARGETS,
	SEQUENCE_COMMAND_TEXT,
	SEQUENCE_COMMAND_SOUND,
	SEQUENCE_COMMAND_GOSUB,
	SEQUENCE_COMMAND_SENTENCE,
	SEQUENCE_COMMAND_REPEAT,
	SEQUENCE_COMMAND_SETDEFAULTS,
	SEQUENCE_COMMAND_MODIFIER,
	SEQUENCE_COMMAND_POSTMODIFIER,
	SEQUENCE_COMMAND_NOOP,

	SEQUENCE_MODIFIER_EFFECT,
	SEQUENCE_MODIFIER_POSITION,
	SEQUENCE_MODIFIER_COLOR,
	SEQUENCE_MODIFIER_COLOR2,
	SEQUENCE_MODIFIER_FADEIN,
	SEQUENCE_MODIFIER_FADEOUT,
	SEQUENCE_MODIFIER_HOLDTIME,
	SEQUENCE_MODIFIER_FXTIME,
	SEQUENCE_MODIFIER_SPEAKER,
	SEQUENCE_MODIFIER_LISTENER,
	SEQUENCE_MODIFIER_TEXTCHANNEL,
};


//---------------------------------------------------------------------------
// sequenceCommandType_e
// 
// Typeerated sequence command types.
//---------------------------------------------------------------------------
EXPORT enum sequenceCommandType_e
{
	SEQUENCE_TYPE_COMMAND,
	SEQUENCE_TYPE_MODIFIER,
};


//---------------------------------------------------------------------------
// sequenceCommandMapping_s
// 
// A mapping of a command enumerated-value to its name.
//---------------------------------------------------------------------------
EXPORT struct sequenceCommandMapping_s
{
	sequenceCommandEnum_e	commandEnum;
	const char*				commandName;
	sequenceCommandType_e	commandType;
};


//---------------------------------------------------------------------------
// sequenceCommandLine_s
// 
// Structure representing a single command (usually 1 line) from a
//	.SEQ file entry.
//---------------------------------------------------------------------------
EXPORT struct sequenceCommandLine_s
{
	int						commandType;		// Specifies the type of command
	client_textmessage_t	clientMessage;		// Text HUD message struct
	char*					speakerName;		// Targetname of speaking entity
	char*					listenerName;		// Targetname of entity being spoken to
	char*					soundFileName;		// Name of sound file to play
	char*					sentenceName;		// Name of sentences.txt to play
	char*					fireTargetNames;	// List of targetnames to fire
	char*					killTargetNames;	// List of targetnames to remove
	float					delay;				// Seconds 'till next command
	int						repeatCount;		// If nonzero, reset execution pointer to top of block (N times, -1 = infinite)
	int						textChannel;		// Display channel on which text message is sent
	int						modifierBitField;	// Bit field to specify what clientmessage fields are valid
	sequenceCommandLine_s*	nextCommandLine;	// Next command (linked list)
};


//---------------------------------------------------------------------------
// sequenceEntry_s
// 
// Structure representing a single command (usually 1 line) from a
//	.SEQ file entry.
//---------------------------------------------------------------------------
EXPORT struct sequenceEntry_s
{
	char*					fileName;		// Name of sequence file without .SEQ extension
	char*					entryName;		// Name of entry label in file
	sequenceCommandLine_s*	firstCommand;	// Linked list of commands in entry
	sequenceEntry_s*		nextEntry;		// Next loaded entry
	qboolean				isGlobal;		// Is entry retained over level transitions?
};


//---------------------------------------------------------------------------
// sentenceEntry_s
// Structure representing a single sentence of a group from a .SEQ
// file entry.  Sentences are identical to entries in sentences.txt, but
// can be unique per level and are loaded/unloaded with the level.
//---------------------------------------------------------------------------
EXPORT struct sentenceEntry_s
{
	char*					data;			// sentence data (ie "We have hostiles" )
	sentenceEntry_s*		nextEntry;		// Next loaded entry
	qboolean				isGlobal;		// Is entry retained over level transitions?
	unsigned int			index;			// this entry's position in the file.
};


//--------------------------------------------------------------------------
// sentenceGroupEntry_s
// Structure representing a group of sentences found in a .SEQ file.
// A sentence group is defined by all sentences with the same name, ignoring
// the number at the end of the sentence name.  Groups enable a sentence
// to be picked at random across a group.
//--------------------------------------------------------------------------
EXPORT struct sentenceGroupEntry_s
{
	char*					groupName;		// name of the group (ie CT_ALERT )
	unsigned int			numSentences;	// number of sentences in group
	sentenceEntry_s*		firstSentence;	// head of linked list of sentences in group
	sentenceGroupEntry_s*	nextEntry;		// next loaded group
};


#if 0
//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
EXPORT sequenceEntry_s* SequenceGet(const char* fileName, const char* entryName);
EXPORT void Sequence_ParseFile(const char* fileName, qboolean isGlobal);
EXPORT void Sequence_OnLevelLoad(const char* mapName);
EXPORT sentenceEntry_s* SequencePickSentence(const char* groupName, int pickMethod, int* picked);
#endif

#pragma endregion Sequence.h

#pragma region netadr.h

EXPORT enum netadrtype_t : uint32_t
{
	NA_UNUSED,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX,
};

EXPORT struct netadr_t
{
	netadrtype_t	type;
	unsigned char	ip[4];
	unsigned char	ipx[10];
	unsigned short	port;
};

#pragma endregion netadr.h

#pragma region delta.h
// all of the contents are amend from ReHLDS

EXPORT inline constexpr uint32_t DELTA_MAX_FIELDS = 56;	// 7*8

EXPORT enum DELTA_TYPES : int32_t
{
	DT_BYTE = (1 << 0),				// A byte
	DT_SHORT = (1 << 1),			// 2 byte field
	DT_FLOAT = (1 << 2),			// A floating point field
	DT_INTEGER = (1 << 3),			// 4 byte integer
	DT_ANGLE = (1 << 4),			// A floating point angle
	DT_TIMEWINDOW_8 = (1 << 5),		// A floating point timestamp relative to server time
	DT_TIMEWINDOW_BIG = (1 << 6),	// A floating point timestamp relative to server time (with more precision and custom multiplier)
	DT_STRING = (1 << 7),			// A null terminated string, sent as 8 byte chars

	DT_SIGNED = (1 << 31),			// sign modificator
};

EXPORT inline constexpr int32_t FDT_MARK = (1 << 0);		// Delta mark for sending

struct delta_t;
EXPORT using encoder_t = void(*)(delta_t*, const std::byte*, const std::byte*) noexcept;

EXPORT struct delta_stats_t
{
	int sendcount;
	int receivedcount;
};

EXPORT struct delta_description_t
{
	int fieldType;
	char fieldName[32];
	int fieldOffset;
	int fieldSize;	// amend by nagi
	int significant_bits;
	float premultiply;
	float postmultiply;
	int16_t flags;
	delta_stats_t stats;
};

#if 0
EXPORT union delta_marked_mask_t
{
	uint8_t u8[8];
	uint32_t u32[2];
	uint64_t u64;	// LUNA: a size of std::uintptr_t?
};
#endif

EXPORT struct delta_t
{
	qboolean dynamic;
	int fieldCount;
	char conditionalencodename[32];
	encoder_t conditionalencode;
	delta_description_t* pdd;
	//delta_marked_mask_t originalMarkedFieldsMask;	// nagi declined this?
	std::uintptr_t originalMarkedFieldsMask;	// #SHOULD_DO_ON_FREE #UNTESTED
};

EXPORT struct delta_encoder_t
{
	delta_encoder_t* next;
	char* name;
	encoder_t conditionalencode;
};

EXPORT struct delta_link_t
{
	delta_link_t* next;
	delta_description_t* delta;
};

EXPORT struct delta_definition_t
{
	char* fieldName;
	size_t fieldOffset;
};

EXPORT struct delta_definition_list_t
{
	delta_definition_list_t* next;
	char* ptypename;
	int numelements;
	delta_definition_t* pdefinition;
};

EXPORT struct delta_registry_t
{
	delta_registry_t* next;
	char* name;
	delta_t* pdesc;
};

EXPORT struct delta_info_t
{
	delta_info_t* next;
	char* name;
	char* loadfile;
	delta_t* delta;
};

#pragma endregion delta.h

#pragma region eiface.h

#ifdef HLDEMO_BUILD
EXPORT inline constexpr auto INTERFACE_VERSION = 001;
#else  // !HLDEMO_BUILD, i.e., regular version of HL
EXPORT inline constexpr auto INTERFACE_VERSION = 140;
#endif // !HLDEMO_BUILD

//
// Defines entity interface between engine and DLLs.
// This header file included by engine files and DLL files.
//
// Before including this header, DLLs must:
//		include progdefs.h
// This is conveniently done for them in extdll.h
//

EXPORT enum ALERT_TYPE : std::int32_t
{
	at_notice,
	at_console,		// same as at_notice, but forces a ConPrintf, not a message box
	at_aiconsole,	// same as at_console, but only shown if developer level is 2!
	at_warning,
	at_error,
	at_logged		// Server print to console ( only in multiplayer games ).
};

// 4-22-98  JOHN: added for use in pfnClientPrintf
EXPORT enum PRINT_TYPE : std::int32_t
{
	print_console,
	print_center,
	print_chat,
};

// For integrity checking of content on clients
EXPORT enum FORCE_TYPE : std::int32_t
{
	force_exactfile,					// File on client must exactly match server's file
	force_model_samebounds,				// For model files only, the geometry must fit in the same bbox
	force_model_specifybounds,			// For model files only, the geometry must fit in the specified bbox
	force_model_specifybounds_if_avail,	// For Steam model files only, the geometry must fit in the specified bbox (if the file is available)
};

// Returned by TraceLine
EXPORT struct TraceResult
{
	int			fAllSolid{};	// if true, plane is not valid
	int			fStartSolid{};	// if true, the initial point was in a solid area
	int			fInOpen{};
	int			fInWater{};
	float		flFraction{};	// time completed, 1.0 = didn't hit anything
	vec3_t		vecEndPos;		// final position
	float		flPlaneDist{};	// LUNA: as the 'd' in ax+by+cz+d=0 standard plane equation.
	vec3_t		vecPlaneNormal;	// surface normal at impact
	edict_t*	pHit{};			// entity the surface is on
	int			iHitgroup{};	// 0 == generic, non zero is specific body part
};

// CD audio status
EXPORT struct CDStatus
{
	qboolean	fPlaying;// is sound playing right now?
	qboolean	fWasPlaying;// if not, CD is paused if WasPlaying is true.
	qboolean	fInitialized;
	qboolean	fEnabled;
	qboolean	fPlayLooping;
	float		cdvolume;
	//BYTE		remap[100];
	int			fCDRom;
	int			fPlayTrack;
};

// Engine hands this to DLLs for functionality callbacks
EXPORT struct enginefuncs_t
{
	int			(*pfnPrecacheModel)			(const char* s){};
	int			(*pfnPrecacheSound)			(const char* s){};
	void		(*pfnSetModel)				(edict_t* e, const char* m){};
	int			(*pfnModelIndex)			(const char* m){};
	int			(*pfnModelFrames)			(int modelIndex){};
	void		(*pfnSetSize)				(edict_t* e, const float* rgflMin, const float* rgflMax){};
	void		(*pfnChangeLevel)			(char* s1, char* s2){};
	void		(*pfnGetSpawnParms)			(edict_t* ent){};
	void		(*pfnSaveSpawnParms)		(edict_t* ent){};
	float		(*pfnVecToYaw)				(const float* rgflVector){};
	void		(*pfnVecToAngles)			(const float* rgflVectorIn, float* rgflVectorOut){};
	void		(*pfnMoveToOrigin)			(edict_t* ent, const float* pflGoal, float dist, int iMoveType){};
	void		(*pfnChangeYaw)				(edict_t* ent){};
	void		(*pfnChangePitch)			(edict_t* ent){};
	edict_t*	(*pfnFindEntityByString)	(edict_t* pEdictStartSearchAfter, const char* pszField, const char* pszValue){};
	int			(*pfnGetEntityIllum)		(edict_t* pEnt){};
	edict_t*	(*pfnFindEntityInSphere)	(edict_t* pEdictStartSearchAfter, const float* org, float rad){};
	edict_t*	(*pfnFindClientInPVS)		(edict_t* pEdict){};
	edict_t*	(*pfnEntitiesInPVS)			(edict_t* pplayer){};
	void		(*pfnMakeVectors)			(const float* rgflVector){};
	void		(*pfnAngleVectors)			(const float* rgflVector, float* forward, float* right, float* up){};
	edict_t*	(*pfnCreateEntity)			(void){};
	void		(*pfnRemoveEntity)			(edict_t* e){};
	edict_t*	(*pfnCreateNamedEntity)		(string_t className){};
	void		(*pfnMakeStatic)			(edict_t* ent){};
	int			(*pfnEntIsOnFloor)			(edict_t* e){};
	int			(*pfnDropToFloor)			(edict_t* e){};
	int			(*pfnWalkMove)				(edict_t* ent, float yaw, float dist, WALKMOVE iMode){};
	void		(*pfnSetOrigin)				(edict_t* e, const float* rgflOrigin){};
	void		(*pfnEmitSound)				(edict_t* entity, SND_CHAN channel, const char* sample, /*int*/float volume, float attenuation, SOUND_FL fFlags, int pitch){};
	void		(*pfnEmitAmbientSound)		(edict_t* entity, const float* pos, const char* samp, float vol, float attenuation, SOUND_FL fFlags, int pitch){};
	void		(*pfnTraceLine)				(const float* v1, const float* v2, TRACE_FL fFlags, edict_t* pentToSkip, TraceResult* ptr){};
	void		(*pfnTraceToss)				(edict_t* pent, edict_t* pentToIgnore, TraceResult* ptr){};
	int			(*pfnTraceMonsterHull)		(edict_t* pEdict, const float* v1, const float* v2, TRACE_FL fFlags, edict_t* pentToSkip, TraceResult* ptr){};
	void		(*pfnTraceHull)				(const float* v1, const float* v2, TRACE_FL fFlags, HULL_TYPES hullNumber, edict_t* pentToSkip, TraceResult* ptr){};
	void		(*pfnTraceModel)			(const float* v1, const float* v2, HULL_TYPES hullNumber, edict_t* pent, TraceResult* ptr){};
	const char*	(*pfnTraceTexture)			(edict_t* pTextureEntity, const float* v1, const float* v2){};
	void		(*pfnTraceSphere)			(const float* v1, const float* v2, TRACE_FL fFlags, float radius, edict_t* pentToSkip, TraceResult* ptr){};
	void		(*pfnGetAimVector)			(edict_t* ent, float speed, float* rgflReturn){};
	void		(*pfnServerCommand)			(const char* str){};
	void		(*pfnServerExecute)			(void){};
	void		(*pfnClientCommand)			(edict_t* pEdict, const char* szFmt, ...){};
	void		(*pfnParticleEffect)		(const float* org, const float* dir, float color, float count){};
	void		(*pfnLightStyle)			(int style, char const* val){};
	int			(*pfnDecalIndex)			(const char* name){};
	PT_CONTENTS	(*pfnPointContents)			(const float* rgflVector){};
	void		(*pfnMessageBegin)			(MSG_DEST msg_dest, ESvcCommands msg_type, const float* pOrigin, edict_t* ed){};
	void		(*pfnMessageEnd)			(void){};
	void		(*pfnWriteByte)				(int iValue){};
	void		(*pfnWriteChar)				(int iValue){};
	void		(*pfnWriteShort)			(int iValue){};
	void		(*pfnWriteLong)				(int iValue){};
	void		(*pfnWriteAngle)			(float flValue){};
	void		(*pfnWriteCoord)			(float flValue){};
	void		(*pfnWriteString)			(const char* sz){};
	void		(*pfnWriteEntity)			(int iValue){};
	void		(*pfnCVarRegister)			(cvar_t* pCvar){};
	float		(*pfnCVarGetFloat)			(const char* szVarName){};
	const char* (*pfnCVarGetString)			(const char* szVarName){};
	void		(*pfnCVarSetFloat)			(const char* szVarName, float flValue){};
	void		(*pfnCVarSetString)			(const char* szVarName, const char* szValue){};
	void		(*pfnAlertMessage)			(ALERT_TYPE atype, const char* szFmt, ...){};
#ifdef HLSDK_3_2_OLD_EIFACE
	void		(*pfnEngineFprintf)			(FILE* pfile, char* szFmt, ...);
	void*		(*pfnPvAllocEntPrivateData)	(edict_t* pEdict, long cb);
#else
	void		(*pfnEngineFprintf)			(void* pfile, const char* szFmt, ...){};
	void*		(*pfnPvAllocEntPrivateData)	(edict_t* pEdict, uint32_t cb){};
#endif
	void*		(*pfnPvEntPrivateData)		(edict_t* pEdict){};
	void		(*pfnFreeEntPrivateData)	(edict_t* pEdict){};
	const char* (*pfnSzFromIndex)			(int iString){};
	string_t	(*pfnAllocString)			(const char* szValue){};
	entvars_t*	(*pfnGetVarsOfEnt)			(edict_t* pEdict){};
	edict_t*	(*pfnPEntityOfEntOffset)	(int iEntOffset){};
	int			(*pfnEntOffsetOfPEntity)	(const edict_t* pEdict){};
	int			(*pfnIndexOfEdict)			(const edict_t* pEdict){};
	edict_t*	(*pfnPEntityOfEntIndex)		(int iEntIndex){};
	edict_t*	(*pfnFindEntityByVars)		(entvars_t* pvars){};
	studiohdr_t* (*pfnGetModelPtr)			(edict_t* pEdict){};
	ESvcCommands(*pfnRegUserMsg)			(const char* pszName, int iSize){};
	void		(*pfnAnimationAutomove)		(const edict_t* pEdict, float flTime){};
	void		(*pfnGetBonePosition)		(const edict_t* pEdict, int iBone, float* rgflOrigin, float* rgflAngles){};
	uintptr_t	(*pfnFunctionFromName)		(const char* pName){};
	const char*	(*pfnNameForFunction)		(std::uintptr_t function){};
	void		(*pfnClientPrintf)			(edict_t* pEdict, PRINT_TYPE ptype, const char* szMsg){}; // JOHN: engine callbacks so game DLL can print messages to individual clients
	void		(*pfnServerPrint)			(const char* szMsg){};
	const char* (*pfnCmd_Args)				(void){};		// these 3 added 
	const char* (*pfnCmd_Argv)				(int argc){};	// so game DLL can easily 
	int			(*pfnCmd_Argc)				(void){};		// access client 'cmd' strings
	void		(*pfnGetAttachment)			(const edict_t* pEdict, int iAttachment, float* rgflOrigin, float* rgflAngles){};
	void		(*pfnCRC32_Init)			(CRC32_t* pulCRC){};
	void        (*pfnCRC32_ProcessBuffer)   (CRC32_t* pulCRC, void* p, int len){};
	void		(*pfnCRC32_ProcessByte)     (CRC32_t* pulCRC, unsigned char ch){};
	CRC32_t		(*pfnCRC32_Final)			(CRC32_t pulCRC){};
	int32_t		(*pfnRandomLong)			(std::int32_t lLow, std::int32_t lHigh){};
	float		(*pfnRandomFloat)			(float flLow, float flHigh){};
	void		(*pfnSetView)				(const edict_t* pClient, const edict_t* pViewent){};
	float		(*pfnTime)					(void){};
	void		(*pfnCrosshairAngle)		(const edict_t* pClient, float pitch, float yaw){};
	std::byte*	(*pfnLoadFileForMe)         (char const* filename, int* pLength){};
	void        (*pfnFreeFile)              (void* buffer){};
	void        (*pfnEndSection)            (const char* pszSectionName){}; // trigger_endsection
	int 		(*pfnCompareFileTime)       (char* filename1, char* filename2, int* iCompare){};
	void        (*pfnGetGameDir)            (char* szGetGameDir){};
	void		(*pfnCvar_RegisterVariable) (cvar_t* variable){};
	void        (*pfnFadeClientVolume)      (const edict_t* pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds){};
	void        (*pfnSetClientMaxspeed)     (const edict_t* pEdict, float fNewMaxspeed){};
	edict_t*	(*pfnCreateFakeClient)		(const char* netname){};	// returns NULL if fake client can't be created
	void		(*pfnRunPlayerMove)			(edict_t* fakeclient, const float* viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, uint8_t impulse, uint8_t msec){};
	int			(*pfnNumberOfEntities)		(void){};
	char*		(*pfnGetInfoKeyBuffer)		(edict_t* e){};	// passing in NULL gets the serverinfo
	char*		(*pfnInfoKeyValue)			(char* infobuffer, char* key){};
	void		(*pfnSetKeyValue)			(char* infobuffer, char* key, char* value){};
	void		(*pfnSetClientKeyValue)		(int clientIndex, char* infobuffer, char* key, char* value){};
	int			(*pfnIsMapValid)			(char* filename){};
	void		(*pfnStaticDecal)			(const float* origin, int decalIndex, int entityIndex, int modelIndex){};
	int			(*pfnPrecacheGeneric)		(const char* s){};
	int			(*pfnGetPlayerUserId)		(edict_t* e){}; // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	void		(*pfnBuildSoundMsg)			(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float* pOrigin, edict_t* ed){};
	int			(*pfnIsDedicatedServer)		(void){};// is this a dedicated server?
	cvar_t*		(*pfnCVarGetPointer)		(const char* szVarName){};
	uint32_t	(*pfnGetPlayerWONId)		(edict_t* e){}; // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients

	// YWB 8/1/99 TFF Physics additions
	void		(*pfnInfo_RemoveKey)		(char* s, const char* key){};
	const char* (*pfnGetPhysicsKeyValue)	(const edict_t* pClient, const char* key){};
	void		(*pfnSetPhysicsKeyValue)	(const edict_t* pClient, const char* key, const char* value){};
	const char* (*pfnGetPhysicsInfoString)	(const edict_t* pClient){};
	uint16_t	(*pfnPrecacheEvent)			(int type, const char* psz){};
	void		(*pfnPlaybackEvent)			(EFEV flags, const edict_t* pInvoker, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2){};

	std::byte*	(*pfnSetFatPVS)				(float* org){};
	std::byte*	(*pfnSetFatPAS)				(float* org){};

	int			(*pfnCheckVisibility)		(const edict_t* entity, unsigned char* pset){};

	void		(*pfnDeltaSetField)			(delta_t* pFields, const char* fieldname){};
	void		(*pfnDeltaUnsetField)		(delta_t* pFields, const char* fieldname){};
	void		(*pfnDeltaAddEncoder)		(char* name, encoder_t conditionalencode){};
	int			(*pfnGetCurrentPlayer)		(void){};
	int			(*pfnCanSkipPlayer)			(const edict_t* player){};
	int			(*pfnDeltaFindField)		(delta_t* pFields, const char* fieldname){};
	void		(*pfnDeltaSetFieldByIndex)	(delta_t* pFields, int fieldNumber){};
	void		(*pfnDeltaUnsetFieldByIndex)(delta_t* pFields, int fieldNumber){};

	void		(*pfnSetGroupMask)			(int mask, int op){};

	int			(*pfnCreateInstancedBaseline) (int classname, entity_state_t* baseline){};
	void		(*pfnCvar_DirectSet)		(cvar_t* var, const char* value){};

	// Forces the client and server to be running with the same version of the specified file
	//  ( e.g., a player model ).
	// Calling this has no effect in single player
	void		(*pfnForceUnmodified)		(FORCE_TYPE type, float* mins, float* maxs, const char* filename){};

	void		(*pfnGetPlayerStats)		(const edict_t* pClient, int* ping, int* packet_loss){};

	void		(*pfnAddServerCommand)		(const char* cmd_name, void (*function) (void)){};

	// For voice communications, set which clients hear eachother.
	// NOTE: these functions take player entity indices (starting at 1).
	qboolean	(*pfnVoice_GetClientListening)(int iReceiver, int iSender){};
	qboolean	(*pfnVoice_SetClientListening)(int iReceiver, int iSender, qboolean bListen){};

	const char* (*pfnGetPlayerAuthId)		(edict_t* e){};

	// PSV: Added for CZ training map
//	const char*		(*pfnKeyNameForBinding)				( const char* pBinding );

	sequenceEntry_s* (*pfnSequenceGet)				(const char* fileName, const char* entryName){};
	sentenceEntry_s* (*pfnSequencePickSentence)		(const char* groupName, int pickMethod, int* picked){};

	// LH: Give access to filesize via filesystem
	int			(*pfnGetFileSize)						(char const* filename){};

	uint32_t	(*pfnGetApproxWavePlayLen)				(const char* filepath){};
	// MDC: Added for CZ career-mode
	int			(*pfnIsCareerMatch)						(void){};

	// BGC: return the number of characters of the localized string referenced by using "label"
	int			(*pfnGetLocalizedStringLength)			(const char* label){};

	// BGC: added to facilitate persistent storage of tutor message decay values for
	// different career game profiles.  Also needs to persist regardless of mp.dll being
	// destroyed and recreated.
	void		(*pfnRegisterTutorMessageShown)			(int mid){};
	int			(*pfnGetTimesTutorMessageShown)			(int mid){};
	void		(*pfnProcessTutorMessageDecayBuffer)	(int* buffer, int bufferLength){};
	void		(*pfnConstructTutorMessageDecayBuffer)	(int* buffer, int bufferLength){};
	void		(*pfnResetTutorMessageDecayData)		(void){};

	// Added 2005/08/11 (no SDK update):
	void		(*pfnQueryClientCvarValue)		(const edict_t* player, const char* cvarName){};

	// Added 2005/11/21 (no SDK update):
	void		(*pfnQueryClientCvarValue2)		(const edict_t* player, const char* cvarName, int requestID){};

	// Added 2009/06/19 (no SDK update):
	int 		(*pfnEngCheckParm)				(const char* pchCmdLineToken, char** pchNextVal){};

#ifdef __METAMOD_BUILD__
	//extra (future updates)
	void* extra_functions[16];
#endif /*__METAMOD_BUILD__*/
};

EXPORT inline enginefuncs_t g_engfuncs = {};

// ONLY ADD NEW FUNCTIONS TO THE END OF THIS STRUCT.  INTERFACE VERSION IS FROZEN AT 138

// Passed to pfnKeyValue
EXPORT struct KeyValueData
{
	const char*	szClassName;	// in: entity classname
	const char*	szKeyName;		// in: name of key
	const char*	szValue;		// in: value of key
	qboolean	fHandled;		// out: DLL sets to true if key-value pair was understood
};

EXPORT struct LEVELLIST
{
	char		mapName[32]{};
	char		landmarkName[32]{};
	edict_t*	pentLandmark{};
	vec3_t		vecLandmarkOrigin;
};
EXPORT inline constexpr size_t MAX_LEVEL_CONNECTIONS = 16;		// These are encoded in the lower 16bits of ENTITYTABLE->flags

EXPORT struct ENTITYTABLE
{
	int			id;				// Ordinal ID of this entity (used for entity <--> pointer conversions)
	edict_t*	pent;			// Pointer to the in-game entity

	int			location;		// Offset from the base data of this entity
	int			size;			// Byte size of this entity's data
	int			flags;			// This could be a short -- bit mask of transitions that this entity is in the PVS of
	string_t	classname;		// entity class name
};

EXPORT enum FENT_TABLE : uint32_t
{
	FENTTABLE_PLAYER = 0x80000000,
	FENTTABLE_REMOVED = 0x40000000,
	FENTTABLE_MOVEABLE = 0x20000000,
	FENTTABLE_GLOBAL = 0x10000000,
};

EXPORT struct SAVERESTOREDATA
{
	char*		pBaseData{};			// Start of all entity save data
	char*		pCurrentData{};			// Current buffer pointer for sequential access
	int			size{};					// Current data size
	int			bufferSize{};			// Total space for data
	int			tokenSize{};			// Size of the linear list of tokens
	int			tokenCount{};			// Number of elements in the pTokens table
	char**		pTokens{};				// Hash table of entity strings (sparse)
	int			currentIndex{};			// Holds a global entity table ID
	int			tableCount{};			// Number of elements in the entity table
	int			connectionCount{};		// Number of elements in the levelList[]
	ENTITYTABLE* pTable{};				// Array of ENTITYTABLE elements (1 for each entity)
	LEVELLIST	levelList[MAX_LEVEL_CONNECTIONS];	// List of connections from this level

	// smooth transition
	int			fUseLandmark{};
	char		szLandmarkName[20]{};	// landmark we'll spawn near in next level
	vec3_t		vecLandmarkOffset;		// for landmark transitions
	float		time{};
	char		szCurrentMapName[32]{};	// To check global entities
};

EXPORT enum FIELDTYPE
{
	FIELD_FLOAT = 0,		// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_ENTITY,			// An entity offset (EOFFSET)
	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EVARS,			// EVARS *
	FIELD_EDICT,			// edict_t *, or edict_t *  (same thing)
	FIELD_VECTOR,			// Any vector
	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_POINTER,			// Arbitrary data pointer... to be removed, use an array of FIELD_CHARACTER
	FIELD_INTEGER,			// Any integer or enum
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)
	FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_TYPECOUNT,		// MUST BE LAST
};

EXPORT inline constexpr uint32_t FTYPEDESC_GLOBAL = 0x0001;		// This field is masked for global entity save/restore

// #GIVEN_UP fucking macros...
#define _FIELD(type,name,fieldtype,count,flags)		{ fieldtype, #name, offsetof(type, name), count, flags }
#define DEFINE_FIELD(type,name,fieldtype)			_FIELD(type, name, fieldtype, 1, 0)
#define DEFINE_ARRAY(type,name,fieldtype,count)		_FIELD(type, name, fieldtype, count, 0)
#define DEFINE_ENTITY_FIELD(name,fieldtype)			_FIELD(entvars_t, name, fieldtype, 1, 0 )
#define DEFINE_ENTITY_GLOBAL_FIELD(name,fieldtype)	_FIELD(entvars_t, name, fieldtype, 1, FTYPEDESC_GLOBAL )
#define DEFINE_GLOBAL_FIELD(type,name,fieldtype)	_FIELD(type, name, fieldtype, 1, FTYPEDESC_GLOBAL )

EXPORT struct TYPEDESCRIPTION
{
	FIELDTYPE	fieldType;
	const char*	fieldName;
	int			fieldOffset;
	short		fieldSize;
	short		flags;
};

EXPORT struct DLL_FUNCTIONS
{
	// Initialize/shutdown the game (one-time call after loading of game .dll )
	void			(*pfnGameInit)			(void);
	int				(*pfnSpawn)				(edict_t* pent);
	void			(*pfnThink)				(edict_t* pent);
	void			(*pfnUse)				(edict_t* pentUsed, edict_t* pentOther);
	void			(*pfnTouch)				(edict_t* pentTouched, edict_t* pentOther);
	void			(*pfnBlocked)			(edict_t* pentBlocked, edict_t* pentOther);
	void			(*pfnKeyValue)			(edict_t* pentKeyvalue, KeyValueData* pkvd);
	void			(*pfnSave)				(edict_t* pent, SAVERESTOREDATA* pSaveData);
	int 			(*pfnRestore)			(edict_t* pent, SAVERESTOREDATA* pSaveData, int globalEntity);
	void			(*pfnSetAbsBox)			(edict_t* pent);

	void			(*pfnSaveWriteFields)	(SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int);
	void			(*pfnSaveReadFields)	(SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int);

	void			(*pfnSaveGlobalState)	(SAVERESTOREDATA*);
	void			(*pfnRestoreGlobalState)(SAVERESTOREDATA*);
	void			(*pfnResetGlobalState)	(void);

	qboolean		(*pfnClientConnect)		(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);

	void			(*pfnClientDisconnect)	(edict_t* pEntity);
	void			(*pfnClientKill)		(edict_t* pEntity);
	void			(*pfnClientPutInServer)	(edict_t* pEntity);
	void			(*pfnClientCommand)		(edict_t* pEntity);
	void			(*pfnClientUserInfoChanged)(edict_t* pEntity, char* infobuffer);

	void			(*pfnServerActivate)	(edict_t* pEdictList, int edictCount, int clientMax);
	void			(*pfnServerDeactivate)	(void);

	void			(*pfnPlayerPreThink)	(edict_t* pEntity);
	void			(*pfnPlayerPostThink)	(edict_t* pEntity);

	void			(*pfnStartFrame)		(void);
	void			(*pfnParmsNewLevel)		(void);
	void			(*pfnParmsChangeLevel)	(void);

	// Returns string describing current .dll.  E.g., TeamFotrress 2, Half-Life
	const char*		(*pfnGetGameDescription)(void);

	// Notify dll about a player customization.
	void            (*pfnPlayerCustomization)(edict_t* pEntity, customization_t* pCustom);

	// Spectator funcs
	void			(*pfnSpectatorConnect)	(edict_t* pEntity);
	void			(*pfnSpectatorDisconnect)(edict_t* pEntity);
	void			(*pfnSpectatorThink)	(edict_t* pEntity);

	// Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.
	void			(*pfnSys_Error)			(const char* error_string);

	void			(*pfnPM_Move)			(playermove_t* ppmove, qboolean server);
	void			(*pfnPM_Init)			(playermove_t* ppmove);
	char			(*pfnPM_FindTextureType)(char* name);
	void			(*pfnSetupVisibility)	(edict_t* pViewEntity, edict_t* pClient, unsigned char** pvs, unsigned char** pas);
	void			(*pfnUpdateClientData)	(const edict_t* ent, qboolean sendweapons, clientdata_t* cd);
	int				(*pfnAddToFullPack)		(entity_state_t* state, int e, edict_t* ent, edict_t* host, int hostflags, int player, unsigned char* pSet);
	void			(*pfnCreateBaseline)	(int player, int eindex, entity_state_t* baseline, edict_t* entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
	void			(*pfnRegisterEncoders)	(void);
	int				(*pfnGetWeaponData)		(edict_t* player, weapon_data_t* info);

	void			(*pfnCmdStart)			(const edict_t* player, const usercmd_t* cmd, unsigned int random_seed);
	void			(*pfnCmdEnd)			(const edict_t* player);

	// Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
	//  size of the response_buffer, so you must zero it out if you choose not to respond.
	int				(*pfnConnectionlessPacket)	(const netadr_t* net_from, const char* args, char* response_buffer, int* response_buffer_size);

	// Enumerates player hulls.  Returns 0 if the hull number doesn't exist, 1 otherwise
	int				(*pfnGetHullBounds)		(int hullnumber, float* mins, float* maxs);

	// Create baselines for certain "unplaced" items.
	void			(*pfnCreateInstancedBaselines) (void);

	// One of the pfnForceUnmodified files failed the consistency check for the specified player
	// Return 0 to allow the client to continue, 1 to force immediate disconnection ( with an optional disconnect message of up to 256 characters )
	qboolean		(*pfnInconsistentFile)	(const edict_t* player, const char* filename, char* disconnect_message);

	// The game .dll should return 1 if lag compensation should be allowed ( could also just set
	//  the sv_unlag cvar.
	// Most games right now should return 0, until client-side weapon prediction code is written
	//  and tested for them.
	int				(*pfnAllowLagCompensation)(void);
};

// Current version.
EXPORT inline constexpr auto NEW_DLL_FUNCTIONS_VERSION = 1;

EXPORT struct NEW_DLL_FUNCTIONS
{
	// Called right before the object's memory is freed. 
	// Calls its destructor.
	void	(*pfnOnFreeEntPrivateData)	(edict_t* pEnt);
	void	(*pfnGameShutdown)			(void);
	qboolean(*pfnShouldCollide)			(edict_t* pentTouched, edict_t* pentOther);

	// Added 2005/08/11 (no SDK update):
	void	(*pfnCvarValue)				(const edict_t* pEnt, const char* value);

	// Added 2005/11/21 (no SDK update):
	//    value is "Bad CVAR request" on failure (i.e that user is not connected or the cvar does not exist).
	//    value is "Bad Player" if invalid player edict.
	void	(*pfnCvarValue2)			(const edict_t* pEnt, int requestID, const char* cvarName, const char* value);
};

EXPORT using NEW_DLL_FUNCTIONS_FN = int (*)(NEW_DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion) noexcept;

EXPORT using APIFUNCTION = int (*)(DLL_FUNCTIONS* pFunctionTable, int interfaceVersion) noexcept;
EXPORT using APIFUNCTION2 = int (*)(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion) noexcept;

#pragma endregion eiface.h
