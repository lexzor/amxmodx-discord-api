// LUNA: module not recommanded. Just directly use std types instead.
export module archtypes;

export import <cstdint>;

export using int16 = std::int16_t;
export using uint16 = std::uint16_t;
export using int32 = std::int32_t;
export using uint32 = std::uint32_t;
export using int64 = std::int64_t;
export using uint64 = std::uint64_t;
export using intp = std::intptr_t;		// intp is an integer that can accomodate a pointer
export using uintp = std::uintptr_t;	// (ie, sizeof(intp) >= sizeof(int) && sizeof(intp) >= sizeof(void *)
