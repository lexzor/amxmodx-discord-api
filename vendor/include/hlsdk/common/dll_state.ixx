export module dll_state;

//DLL State Flags

export inline constexpr auto DLL_INACTIVE = 0;	// no dll
export inline constexpr auto DLL_ACTIVE = 1;	// dll is running
export inline constexpr auto DLL_PAUSED = 2;	// dll is paused
export inline constexpr auto DLL_CLOSE = 3;		// closing down dll
export inline constexpr auto DLL_TRANS = 4; 	// Level Transition

// DLL Pause reasons

export inline constexpr auto DLL_NORMAL = 0;   // User hit Esc or something.
export inline constexpr auto DLL_QUIT = 4;		// Quit now
export inline constexpr auto DLL_RESTART = 6;   // Switch to launcher for linux, does a quit but returns 1

// DLL Substate info ( not relevant )
export inline constexpr auto ENG_NORMAL = (1 << 0);
