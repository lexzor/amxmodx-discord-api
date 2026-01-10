export module cdll_dll;

export inline constexpr auto MAX_WEAPONS = 32;		// ???

export inline constexpr auto MAX_WEAPON_SLOTS = 5;	// hud item selection slots
export inline constexpr auto MAX_ITEM_TYPES = 6;	// hud item selection slots

export inline constexpr auto MAX_ITEMS = 5;	// hard coded item types

export inline constexpr auto HIDEHUD_WEAPONS = (1 << 0);
export inline constexpr auto HIDEHUD_FLASHLIGHT = (1 << 1);
export inline constexpr auto HIDEHUD_ALL = (1 << 2);
export inline constexpr auto HIDEHUD_HEALTH = (1 << 3);
export inline constexpr auto HIDEHUD_TIMER = (1 << 4);
export inline constexpr auto HIDEHUD_MONEY = (1 << 5);
export inline constexpr auto HIDEHUD_CROSSHAIR = (1 << 6);

export inline constexpr auto MAX_AMMO_TYPES = 32;		// ???
export inline constexpr auto MAX_AMMO_SLOTS = 32;		// not really slots

export inline constexpr auto HUD_PRINTNOTIFY = 1;
export inline constexpr auto HUD_PRINTCONSOLE = 2;
export inline constexpr auto HUD_PRINTTALK = 3;
export inline constexpr auto HUD_PRINTCENTER = 4;

export inline constexpr auto DEFAULT_FOV = 90;	// the default field of view

export inline constexpr auto WEAPON_SUIT = 31;
