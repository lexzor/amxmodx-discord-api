export module mathlib;

export import <cassert>;
export import <cmath>;
export import <concepts>;
export import <numbers>;

export import vector;


export constexpr bool IS_NAN(std::floating_point auto fl) noexcept { return fl != fl; }

// LUNA: one should not rely on those old asm trick with a modern compiler.
