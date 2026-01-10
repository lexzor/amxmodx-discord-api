export module progs;

export import event_args;
export import event_flags;
export import progdefs;

// 16 simultaneous events, max
export inline constexpr auto MAX_EVENT_QUEUE = 64;

export inline constexpr auto DEFAULT_EVENT_RESENDS = 1;

export struct event_info_s
{
	unsigned short index;			  // 0 implies not in use

	short packet_index;      // Use data from state info for entity in delta_packet .  -1 implies separate info based on event
	// parameter signature
	short entity_index;      // The edict this event is associated with

	float fire_time;        // if non-zero, the time when the event should be fired ( fixed up on the client )

	event_args_t args;

	// CLIENT ONLY	
	int	  flags;			// Reliable or not, etc.

};

export using event_info_t = event_info_s;

export struct event_state_s
{
	event_info_t ei[MAX_EVENT_QUEUE];
};

export using event_state_t = event_state_s;

// #GIVEN_UP macros
//#define	STRUCT_FROM_LINK(l,t,m) ((t *)((byte *)l - (int)&(((t *)0)->m)))
//#define	EDICT_FROM_AREA(l) STRUCT_FROM_LINK(l,edict_t,area)

//============================================================================

//extern char *pr_strings;
//extern globalvars_t gGlobalVariables;

//============================================================================

//edict_t *ED_Alloc(void);
//void ED_Free(edict_t *ed);
//void ED_LoadFromFile(char *data);

//edict_t *EDICT_NUM(int n);
//int NUM_FOR_EDICT(const edict_t *e);

//#define PROG_TO_EDICT(e) ((edict_t *)((byte *)sv.edicts + e))
