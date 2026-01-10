export module hlsdk:decompiled;

#ifdef __INTELLISENSE__
import std;
using namespace std;
#else
import std.compat;	// #MSVC_BUG_STDCOMPAT
#endif

import :engine;


#pragma region enums.h

// Used as array indexer
export enum netsrc_t : uint32_t
{
	NS_CLIENT = 0,
	NS_SERVER,
	NS_MULTICAST,	// xxxMO
	NS_MAX
};

#pragma endregion enums.h

#pragma region sizebuf.h

export struct sizebuf_t
{
	const char* buffername{};
	uint32_t flags{};
	std::byte* data{};
	uint32_t maxsize{};
	uint32_t cursize{};
};

#pragma endregion sizebuf.h

#pragma region net.h

// NETWORKING INFO

// Max length of a reliable message
export inline constexpr auto MAX_MSGLEN = 5990; // 10 reserved for fragheader? LUNA: increased by 2000 to 5990 in HL25

// Max length of unreliable message
export inline constexpr auto MAX_DATAGRAM = 6000;	// LUNA: increased by 2000 to 6000 in HL25

// LUNA: It's 1400 without ReHLDS_FIX macro??? But it's correct with 1024.
export inline constexpr auto FRAGMENT_MAX_SIZE = 1024;

// 0 == regular, 1 == file stream
export enum ENetFragTypes : uint32_t
{
	FRAG_NORMAL_STREAM = 0,
	FRAG_FILE_STREAM,

	MAX_STREAMS
};

export enum ENetFlow : uint32_t
{
	FLOW_OUTGOING = 0,
	FLOW_INCOMING,

	MAX_FLOWS
};

// Generic fragment structure
export struct fragbuf_t
{
	// Next buffer in chain
	struct fragbuf_t* next{};
	// Id of this buffer
	int32_t bufferid{};
	// Message buffer where raw data is stored
	sizebuf_t frag_message{};
	// The actual data sits here
	std::byte frag_message_buf[FRAGMENT_MAX_SIZE]{};
	// Is this a file buffer?
	qboolean isfile{};
	// Is this file buffer from memory ( custom decal, etc. ).
	qboolean isbuffer{};
	qboolean iscompressed{};
	// Name of the file to save out on remote host
	char filename[260]{};
	// Offset in file from which to read data
	int32_t foffset{};
	// Size of data to read at that offset
	uint32_t size{};
};

// Waiting list of fragbuf chains
export struct fragbufwaiting_t
{
	// Next chain in waiting list
	struct fragbufwaiting_t* next{};
	// Number of buffers in this chain
	int32_t fragbufcount{};
	// The actual buffers
	fragbuf_t* fragbufs{};
};

// Message data
export struct flowstats_t
{
	// Size of message sent/received
	uint32_t size{};
	// Time that message was sent/received
	double time{};
};

export inline constexpr auto MAX_LATENT = 32;

export struct flow_t
{
	// Data for last MAX_LATENT messages
	flowstats_t stats[MAX_LATENT]{};
	// Current message position
	int32_t current{};
	// Time when we should recompute k/sec data
	double nextcompute{};
	// Average data
	float kbytespersec{};
	float avgkbytespersec{};
};

// Network Connection Channel
export struct netchan_t
{
	// NS_SERVER or NS_CLIENT, depending on channel.
	netsrc_t sock;

	// Address this channel is talking to.
	netadr_t remote_address;

	int32_t player_slot;
	// For timeouts.  Time last message was received.
	float last_received;
	// Time when channel was connected.
	float connect_time;

	// Bandwidth choke
	// Bytes per second
	double rate;
	// If realtime > cleartime, free to send next packet
	double cleartime;

	// Sequencing variables
	//
	// Increasing count of sequence numbers
	int32_t incoming_sequence;
	// # of last outgoing message that has been ack'd.
	int32_t incoming_acknowledged;
	// Toggles T/F as reliable messages are received.
	int32_t incoming_reliable_acknowledged;
	// single bit, maintained local
	int32_t incoming_reliable_sequence;
	// Message we are sending to remote
	int32_t outgoing_sequence;
	// Whether the message contains reliable payload, single bit
	int32_t reliable_sequence;
	// Outgoing sequence number of last send that had reliable data
	int32_t last_reliable_sequence;

	void* connection_status;
	int32_t(*pfnNetchan_Blocksize)(void*) noexcept;

	// Staging and holding areas
	sizebuf_t message;
	std::byte message_buf[MAX_MSGLEN];

	// Reliable message buffer. We keep adding to it until reliable is acknowledged. Then we clear it.
	uint32_t reliable_length;
	std::byte reliable_buf[MAX_MSGLEN];

	// Waiting list of buffered fragments to go onto queue. Multiple outgoing buffers can be queued in succession.
	fragbufwaiting_t* waitlist[MAX_STREAMS];

	// Is reliable waiting buf a fragment?
	int32_t reliable_fragment[MAX_STREAMS];
	// Buffer id for each waiting fragment
	uint32_t reliable_fragid[MAX_STREAMS];

	// The current fragment being set
	fragbuf_t* fragbufs[MAX_STREAMS];
	// The total number of fragments in this stream
	int32_t fragbufcount[MAX_STREAMS];

	// Position in outgoing buffer where frag data starts
	int16_t frag_startpos[MAX_STREAMS];
	// Length of frag data in the buffer
	int16_t frag_length[MAX_STREAMS];

	// Incoming fragments are stored here
	fragbuf_t* incomingbufs[MAX_STREAMS];
	// Set to true when incoming data is ready
	qboolean incomingready[MAX_STREAMS];

	// Only referenced by the FRAG_FILE_STREAM component
	// Name of file being downloaded
	char incomingfilename[260];

	void* tempbuffer;
	int32_t tempbuffersize;

	// Incoming and outgoing flow metrics
	flow_t flow[MAX_FLOWS];
};

#pragma endregion net.h

#pragma region delta_packet.h

export struct packet_entities_t
{
	uint32_t num_entities{};
	std::byte flags[32]{};
	entity_state_t* entities{};
};

#pragma endregion delta_packet.h

// Should be in client.dll
#pragma region event_args.h

// Event was invoked with stated origin
export inline constexpr auto FEVENT_ORIGIN = (1 << 0);

// Event was invoked with stated angles
export inline constexpr auto FEVENT_ANGLES = (1 << 1);

export struct event_args_t
{
	uint32_t	flags{};

	// Transmitted
	uint32_t	entindex{};

	Vector		origin{};
	Angles		angles{};
	Vector		velocity{};

	qboolean	ducking{};

	float		fparam1{};
	float		fparam2{};

	int32_t		iparam1{};
	int32_t		iparam2{};

	qboolean	bparam1{};
	qboolean	bparam2{};
};

#pragma endregion event_args.h

#pragma region progs.h

// Seems cl-ish but the other part in ReHLDS header is sv-ish.
export struct event_info_t
{
	uint16_t index{};			// 0 implies not in use

	int16_t packet_index{};	// Use data from state info for entity in delta_packet .  -1 implies separate info based on event
	// parameter signature
	int16_t entity_index{};	// The edict this event is associated with

	float fire_time{};		// if non-zero, the time when the event should be fired ( fixed up on the client )

	event_args_t args{};

	// CLIENT ONLY	
	uint32_t flags{};			// Reliable or not, etc.
};

// 16 simultaneous events, max
export inline constexpr auto MAX_EVENT_QUEUE = 64;

export struct event_state_t
{
	event_info_t ei[MAX_EVENT_QUEUE]{};
};

#pragma endregion progs.h

#pragma region userid_rehlds.h

// Authentication types
export enum AUTH_IDTYPE : uint32_t
{
	AUTH_IDTYPE_UNKNOWN = 0,
	AUTH_IDTYPE_STEAM = 1,
	AUTH_IDTYPE_VALVE = 2,
	AUTH_IDTYPE_LOCAL = 3
};

export struct USERID_t
{
	AUTH_IDTYPE idtype{};
	uint64_t m_SteamID{};
	uint32_t clientip{};
};

#pragma endregion userid_rehlds.h

#pragma region server.h

export struct client_frame_t
{
	double senttime{};
	float ping_time{};
	clientdata_t clientdata{};
	weapon_data_t weapondata[64]{};
	packet_entities_t entities{};
};

// LUNA: should have expose this struct to us..
export struct client_t
{
	qboolean active;
	qboolean spawned;
	qboolean fully_connected;
	qboolean connected;
	qboolean uploading;
	qboolean hasusrmsgs;
	qboolean has_force_unmodified;
	netchan_t netchan;
	int32_t chokecount;
	int32_t delta_sequence;
	qboolean fakeclient;
	qboolean proxy;
	usercmd_t lastcmd;
	double connecttime;
	double cmdtime;
	double ignorecmdtime;
	float latency;
	float packet_loss;
	double localtime;
	double nextping;
	double svtimebase;
	sizebuf_t datagram;
	std::byte datagram_buf[MAX_DATAGRAM];
	double connection_started;
	double next_messagetime;
	double next_messageinterval;
	qboolean send_message;
	qboolean skip_message;
	client_frame_t* frames;
	event_state_t events;
	edict_t* edict;
	edict_t const* pViewEntity;
	uint32_t userid;
	USERID_t network_userid;
	char userinfo[MAX_INFO_STRING];	// 25392
	qboolean sendinfo;		// 25648?
	float sendinfo_time;	// 25652?
	char hashedcdkey[64];	// 25656?
	char name[32];	// 25720, pev->netname?
	int32_t topcolor;
	int32_t bottomcolor;
	int32_t entityId;
	resource_t resourcesonhand;
	resource_t resourcesneeded;
	void* upload;	// FileHandle_t
	qboolean uploaddoneregistering;
	customization_t customdata;
	uint32_t crcValue;
	qboolean lw;	// cl_lw
	qboolean lc;	// furthest confirmed offset.
	char physinfo[MAX_INFO_STRING];
	qboolean m_bLoopback;
	int32_t m_VoiceStreams[2];	// unknown meaning of this size 2.
	double m_lastvoicetime;	// LUNA: unused? didn't change during test.
	int32_t m_sendrescount;
	qboolean m_bSentNewResponse;
	//movevars_t movevars;	// LUNA: not present in HL25(9980), increasing 136 bytes because of padding.
};

static_assert(sizeof(client_t) == 26504, "Anniversary 9980");

// maintypes.h says it should be in engine/server.h
export enum server_state_t : uint32_t
{
	ss_dead = 0,
	ss_loading = 1,
	ss_active = 2,
};

#pragma endregion server.h (Part I)

#pragma region server_static.h

export struct server_log_t
{
	qboolean active{};
	qboolean net_log_{};
	netadr_t net_address_{};
	void* file{};
};

export struct server_stats_t
{
	int32_t num_samples{};
	int32_t at_capacity{};
	int32_t at_empty{};
	float capacity_percent{};
	float empty_percent{};
	int32_t minusers{};
	int32_t maxusers{};
	float cumulative_occupancy{};
	float occupancy{};
	int32_t num_sessions{};
	float cumulative_sessiontime{};
	float average_session_len{};
	float cumulative_latency{};
	float average_latency{};
};

export struct server_static_t
{
	qboolean dll_initialized{};
	client_t* clients{};	// build span with maxclientslimit.
	uint32_t maxclients{};
	uint32_t maxclientslimit{};
	int32_t spawncount{};
	int32_t serverflags{};
	server_log_t log{};
	double next_cleartime{};
	double next_sampletime{};
	server_stats_t stats{};
	qboolean isSecure{};
};

// Corresponding to g_psvs in ReHLDS.
export extern "C++" inline server_static_t* gpServerStatics = nullptr;

#pragma endregion server_static.h

#pragma region consistency.h

export inline constexpr auto MAX_CONSISTENCY_LIST = 512;

export struct consistency_t
{
	char const* filename{};
	qboolean issound{};
	int32_t orig_index{};
	int32_t value{};
	FORCE_TYPE check_type{};
	Vector mins{};
	Vector maxs{};
};

#pragma endregion consistency.h

#pragma region qlimits.h

export inline constexpr auto MAX_MODEL_INDEX_BITS = (int16_t)9;	// sent as a short
export inline constexpr auto MAX_MODELS = (1 << MAX_MODEL_INDEX_BITS);
export inline constexpr auto MAX_SOUND_INDEX_BITS = (int16_t)9;
export inline constexpr auto MAX_SOUNDS = (1 << MAX_SOUND_INDEX_BITS);
export inline constexpr auto MAX_SOUNDS_HASHLOOKUP_SIZE = (MAX_SOUNDS * 2 - 1);

export inline constexpr auto MAX_GENERIC_INDEX_BITS = (int16_t)9;
export inline constexpr auto MAX_GENERIC = (1 << MAX_GENERIC_INDEX_BITS);
export inline constexpr auto MAX_DECAL_INDEX_BITS = (int16_t)9;
export inline constexpr auto MAX_BASE_DECALS = (1 << MAX_DECAL_INDEX_BITS);

export inline constexpr auto MAX_EVENTS = 256;

export inline constexpr auto MAX_LIGHTSTYLE_INDEX_BITS = (int16_t)6;
export inline constexpr auto MAX_LIGHTSTYLES = (1 << MAX_LIGHTSTYLE_INDEX_BITS);
export inline constexpr auto MAX_LIGHTSTYLE_SIZE = size_t{ 64 };

#pragma endregion qlimits.h

#pragma region event.h

export struct event_t
{
	uint16_t index{};
	const char* filename{};
	uint32_t filesize{};
	const char* pszScript{};
};

#pragma endregion event.h

#pragma region inst_baseline.h

export inline constexpr int NUM_BASELINES = 64;

export struct extra_baselines_t
{
	int32_t number{};
	string_t classname[NUM_BASELINES]{};
	entity_state_t baseline[NUM_BASELINES]{};
};

#pragma endregion inst_baseline.h

#pragma region server.h (Part II)

export struct server_t
{
	qboolean active;
	qboolean paused;
	qboolean loadgame;
	double time;
	double oldtime;
	int32_t lastcheck;
	double lastchecktime;
	char name[64];
	char oldname[64];
	char startspot[64];
	char modelname[64];
	model_t* worldmodel;
	CRC32_t worldmapCRC;
	unsigned char clientdllmd5[16];
	resource_t resourcelist[MAX_RESOURCE_LIST];
	int32_t num_resources;
	consistency_t consistency_list[MAX_CONSISTENCY_LIST];
	int32_t num_consistency;
	const char* model_precache[MAX_MODELS];
	model_t* models[MAX_MODELS];
	unsigned char model_precache_flags[MAX_MODELS];
	event_t event_precache[MAX_EVENTS];
	const char* sound_precache[MAX_SOUNDS];
	int16_t sound_precache_hashedlookup[MAX_SOUNDS_HASHLOOKUP_SIZE];
	qboolean sound_precache_hashedlookup_built;
	const char* generic_precache[MAX_GENERIC];
	char generic_precache_names[MAX_GENERIC][64];
	int32_t num_generic_names;
	const char* lightstyles[MAX_LIGHTSTYLES];
	int32_t num_edicts;
	int32_t max_edicts;
	edict_t* edicts;
	entity_state_t* baselines;
	extra_baselines_t* instance_baselines;
	server_state_t state;
	sizebuf_t datagram;
	std::byte datagram_buf[MAX_DATAGRAM];
	sizebuf_t reliable_datagram;
	std::byte reliable_datagram_buf[MAX_DATAGRAM];
	sizebuf_t multicast;
	std::byte multicast_buf[1024];
	sizebuf_t spectator;
	std::byte spectator_buf[1024];
	sizebuf_t signon;
	std::byte signon_data[32768];
};

// Corresponding to g_psv in ReHLDS.
export extern "C++" inline server_t* gpServerVars = nullptr;

#pragma endregion server.h (Part II)
