export module con_nprint;

export struct con_nprint_s
{
	int		index;			// Row #
	float	time_to_live;	// # of seconds before it dissappears
	float	color[3];		// RGB colors ( 0.0 -> 1.0 scale )
};

export using con_nprint_t = con_nprint_s;

export void Con_NPrintf(int idx, char *fmt, ...);
export void Con_NXPrintf(con_nprint_t *info, char *fmt, ...);
