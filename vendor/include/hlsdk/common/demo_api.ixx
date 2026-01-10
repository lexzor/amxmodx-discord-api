export module demo_api;

export struct demo_api_s
{
	int		(*IsRecording)		(void);
	int		(*IsPlayingback)	(void);
	int		(*IsTimeDemo)		(void);
	void	(*WriteBuffer)		(int size, unsigned char *buffer);
};

export using demo_api_t = demo_api_s;

// #NO_URGENT
//extern demo_api_t demoapi;
