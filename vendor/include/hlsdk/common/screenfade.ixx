export module screenfade;

export import const_;

export struct screenfade_s
{
	float		fadeSpeed;		// How fast to fade (tics / second) (+ fade in, - fade out)
	float		fadeEnd;		// When the fading hits maximum
	float		fadeTotalEnd;	// Total End Time of the fade (used for FFADE_OUT)
	float		fadeReset;		// When to reset to not fading (for fadeout and hold)
	byte		fader, fadeg, fadeb, fadealpha;	// Fade color
	int			fadeFlags;		// Fading flags
};

export using screenfade_t = screenfade_s;
