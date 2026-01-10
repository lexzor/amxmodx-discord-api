export module hltv;

// hltv.h
// all shared consts between server, clients and proxy

export inline constexpr auto TYPE_CLIENT = 0;	// client is a normal HL client (default)
export inline constexpr auto TYPE_PROXY = 1;	// client is another proxy
export inline constexpr auto TYPE_COMMENTATOR = 3;	// client is a commentator
export inline constexpr auto TYPE_DEMO = 4;	// client is a demo file
// sub commands of svc_hltv:
export inline constexpr auto HLTV_ACTIVE = 0;	// tells client that he's an spectator and will get director commands
export inline constexpr auto HLTV_STATUS = 1;	// send status infos about proxy 
export inline constexpr auto HLTV_LISTEN = 2;	// tell client to listen to a multicast stream

// sub commands of svc_director:
export inline constexpr auto DRC_CMD_NONE = 0;	// NULL director command
export inline constexpr auto DRC_CMD_START = 1;	// start director mode
export inline constexpr auto DRC_CMD_EVENT = 2;	// informs about director command
export inline constexpr auto DRC_CMD_MODE = 3;	// switches camera modes
export inline constexpr auto DRC_CMD_CAMERA = 4;	// sets camera registers
export inline constexpr auto DRC_CMD_TIMESCALE = 5;	// sets time scale
export inline constexpr auto DRC_CMD_MESSAGE = 6;	// send HUD centerprint
export inline constexpr auto DRC_CMD_SOUND = 7;	// plays a particular sound
export inline constexpr auto DRC_CMD_STATUS = 8;	// status info about broadcast
export inline constexpr auto DRC_CMD_BANNER = 9;	// banner file name for HLTV gui
export inline constexpr auto DRC_CMD_FADE = 10;	// send screen fade command
export inline constexpr auto DRC_CMD_SHAKE = 11;	// send screen shake command
export inline constexpr auto DRC_CMD_STUFFTEXT = 12;	// like the normal svc_stufftext but as director command

export inline constexpr auto DRC_CMD_LAST = 12;



// HLTV_EVENT event flags
export inline constexpr auto DRC_FLAG_PRIO_MASK = 0x0F;	// priorities between 0 and 15 (15 most important)
export inline constexpr auto DRC_FLAG_SIDE = (1 << 4);	// 
export inline constexpr auto DRC_FLAG_DRAMATIC = (1 << 5);	// is a dramatic scene
export inline constexpr auto DRC_FLAG_SLOWMOTION = (1 << 6);  // would look good in SloMo
export inline constexpr auto DRC_FLAG_FACEPLAYER = (1 << 7);  // player is doning something (reload/defuse bomb etc)
export inline constexpr auto DRC_FLAG_INTRO = (1 << 8);	// is a introduction scene
export inline constexpr auto DRC_FLAG_FINAL = (1 << 9);	// is a final scene
export inline constexpr auto DRC_FLAG_NO_RANDOM = (1 << 10);	// don't randomize event data


export inline constexpr auto MAX_DIRECTOR_CMD_PARAMETERS = 4;
export inline constexpr auto MAX_DIRECTOR_CMD_STRING = 128;
