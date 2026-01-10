export module in_buttons;

export inline constexpr auto IN_ATTACK = (1 << 0);
export inline constexpr auto IN_JUMP = (1 << 1);
export inline constexpr auto IN_DUCK = (1 << 2);
export inline constexpr auto IN_FORWARD = (1 << 3);
export inline constexpr auto IN_BACK = (1 << 4);
export inline constexpr auto IN_USE = (1 << 5);
export inline constexpr auto IN_CANCEL = (1 << 6);
export inline constexpr auto IN_LEFT = (1 << 7);
export inline constexpr auto IN_RIGHT = (1 << 8);
export inline constexpr auto IN_MOVELEFT = (1 << 9);
export inline constexpr auto IN_MOVERIGHT = (1 << 10);
export inline constexpr auto IN_ATTACK2 = (1 << 11);
export inline constexpr auto IN_RUN = (1 << 12);
export inline constexpr auto IN_RELOAD = (1 << 13);
export inline constexpr auto IN_ALT1 = (1 << 14);
export inline constexpr auto IN_SCORE = (1 << 15);   // Used by client.dll for when scoreboard is held down
