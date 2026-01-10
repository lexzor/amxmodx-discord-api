export module pm_shared;

// #STUB_FN
//void PM_Init(struct playermove_s *ppmove);
//void PM_Move(struct playermove_s *ppmove, int server);
//char PM_FindTextureType(char *name);

// Spectator Movement modes (stored in pev->iuser1, so the physics code can get at them)
export inline constexpr auto OBS_NONE = 0;
export inline constexpr auto OBS_CHASE_LOCKED = 1;
export inline constexpr auto OBS_CHASE_FREE = 2;
export inline constexpr auto OBS_ROAMING = 3;
export inline constexpr auto OBS_IN_EYE = 4;
export inline constexpr auto OBS_MAP_FREE = 5;
export inline constexpr auto OBS_MAP_CHASE = 6;
