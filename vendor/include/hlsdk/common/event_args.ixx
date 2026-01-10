export module event_args;

// Event was invoked with stated origin
export inline constexpr auto FEVENT_ORIGIN = (1 << 0);

// Event was invoked with stated angles
export inline constexpr auto FEVENT_ANGLES = (1 << 1);

export struct event_args_s
{
	int		flags;

	// Transmitted
	int		entindex;

	float	origin[3];
	float	angles[3];
	float	velocity[3];

	int		ducking;

	float	fparam1;
	float	fparam2;

	int		iparam1;
	int		iparam2;

	int		bparam1;
	int		bparam2;
};

export using event_args_t = event_args_s;
