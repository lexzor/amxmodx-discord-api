export module shake;

export import const_;

// Screen / View effects

// screen shake
//extern int gmsgShake;

// This structure is sent over the net to describe a screen shake event
export struct ScreenShake
{
	unsigned short	amplitude;		// FIXED 4.12 amount of shake
	unsigned short 	duration;		// FIXED 4.12 seconds duration
	unsigned short	frequency;		// FIXED 8.8 noise frequency (low frequency is a jerk,high frequency is a rumble)
};

//extern void V_ApplyShake(float *origin, float *angles, float factor);
//extern void V_CalcShake(void);
//extern int V_ScreenShake(const char *pszName, int iSize, void *pbuf);
//extern int V_ScreenFade(const char *pszName, int iSize, void *pbuf);


// Fade in/out
//extern int gmsgFade;

export inline constexpr auto FFADE_IN = 0x0000;		// LUNA: Fade into game world.
export inline constexpr auto FFADE_OUT = 0x0001;		// LUNA: Fade into color screen
export inline constexpr auto FFADE_MODULATE = 0x0002;		// Modulate (don't blend)
export inline constexpr auto FFADE_STAYOUT = 0x0004;		// ignores the duration, stays faded out until new ScreenFade message received

// This structure is sent over the net to describe a screen fade event
export struct ScreenFade
{
	unsigned short 	duration;		// FIXED 4.12 seconds duration
	unsigned short 	holdTime;		// FIXED 4.12 seconds duration until reset (fade & hold)
	short			fadeFlags;		// flags
	byte			r, g, b, a;		// fade to color ( max alpha )
};
