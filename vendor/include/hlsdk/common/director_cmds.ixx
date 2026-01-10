export module director_cmds;

// director_cmds.h
// sub commands for svc_director

export inline constexpr auto DRC_ACTIVE = 0;	// tells client that he's an spectator and will get director command
export inline constexpr auto DRC_STATUS = 1;	// send status infos about proxy 
export inline constexpr auto DRC_CAMERA = 2;	// set the actual director camera position
export inline constexpr auto DRC_EVENT = 3;		// informs the dircetor about ann important game event


export inline constexpr auto DRC_FLAG_PRIO_MASK = 0x0F;	//	priorities between 0 and 15 (15 most important)
export inline constexpr auto DRC_FLAG_SIDE = (1 << 4);
export inline constexpr auto DRC_FLAG_DRAMATIC = (1 << 5);



// commands of the director API function CallDirectorProc(...)

export inline constexpr auto DRCAPI_NOP = 0;	// no operation
export inline constexpr auto DRCAPI_ACTIVE = 1;	// de/acivates director mode in engine
export inline constexpr auto DRCAPI_STATUS = 2;   // request proxy information
export inline constexpr auto DRCAPI_SETCAM = 3;	// set camera n to given position and angle
export inline constexpr auto DRCAPI_GETCAM = 4;	// request camera n position and angle
export inline constexpr auto DRCAPI_DIRPLAY = 5;	// set director time and play with normal speed
export inline constexpr auto DRCAPI_DIRFREEZE = 6;	// freeze directo at this time
export inline constexpr auto DRCAPI_SETVIEWMODE = 7;	// overview or 4 cameras 
export inline constexpr auto DRCAPI_SETOVERVIEWPARAMS = 8;	// sets parameter for overview mode
export inline constexpr auto DRCAPI_SETFOCUS = 9;	// set the camera which has the input focus
export inline constexpr auto DRCAPI_GETTARGETS = 10;	// queries engine for player list
export inline constexpr auto DRCAPI_SETVIEWPOINTS = 11;	// gives engine all waypoints
