export module studio_event;

export struct mstudioevent_s
{
	int 				frame;
	int					event;
	int					type;
	char				options[64];
};

export using mstudioevent_t = mstudioevent_s;
