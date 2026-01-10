#if !defined(INCLUDED_IN_MODULE) || defined(__INTELLISENSE__)
#pragma once

#include <cstdint>

#include <concepts>

#else

import std;

import vector;

#endif

#ifndef EXPORT
#define EXPORT
#endif

// Custom Entities
// LUNA: forget it, just beams.

// Start/End Entity is encoded as 12 bits of entity index, and 4 bits of attachment (4:12)

EXPORT template <std::integral T> inline constexpr [[nodiscard]]
T BEAMENT_ENTITY(T x) noexcept
{
	return x & (T)0xFFF;
}

EXPORT template <std::integral T> inline constexpr [[nodiscard]]
T BEAMENT_ATTACHMENT(T x) noexcept
{
	return (x >> 12) & (T)0xF;
}

// Beam types, encoded as a byte
EXPORT enum EBeamTypes : std::uint8_t
{
	BEAM_POINTS = 0,
	BEAM_ENTPOINT,
	BEAM_ENTS,
	BEAM_HOSE,
};

EXPORT enum EBeamFlags : std::uint8_t
{
	BEAM_FSINE = 0x10,
	BEAM_FSOLID = 0x20,
	BEAM_FSHADEIN = 0x40,
	BEAM_FSHADEOUT = 0x80,
};
