export module cl_entity;

export import com_model;
export import entity_state;
export import progs;


struct cl_entity_t;

export struct efrag_t
{
	mleaf_t* leaf;
	efrag_t* leafnext;
	cl_entity_t* entity;
	efrag_t* entnext;
};

export struct mouth_t
{
	byte	mouthopen;		// 0 = mouth closed, 255 = mouth agape
	byte	sndcount;		// counter for running average
	int		sndavg;			// running average
};

export struct latchedvars_t
{
	float					prevanimtime;
	float					sequencetime;
	byte					prevseqblending[2];
	vec3_t					prevorigin;
	vec3_t					prevangles;

	int						prevsequence;
	float					prevframe;

	byte					prevcontroller[4];
	byte					prevblending[2];
};

export struct position_history_t
{
	// Time stamp for this movement
	float					animtime;

	vec3_t					origin;
	vec3_t					angles;
};

export inline constexpr auto HISTORY_MAX = 64;  // Must be power of 2
export inline constexpr auto HISTORY_MASK = (HISTORY_MAX - 1);

export struct cl_entity_t
{
	int						index;      // Index into cl_entities ( should match actual slot, but not necessarily )

	qboolean				player;     // True if this entity is a "player"

	entity_state_t			baseline;   // The original state from which to delta during an uncompressed message
	entity_state_t			prevstate;  // The state information from the penultimate message received from the server
	entity_state_t			curstate;   // The state information from the last message received from server

	int						current_position;  // Last received history update index
	position_history_t		ph[HISTORY_MAX];   // History of position and angle updates for this player

	mouth_t					mouth;			// For synchronizing mouth movements.

	latchedvars_t			latched;		// Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	float					lastmove;

	// Actual render position and angles
	vec3_t					origin;
	vec3_t					angles;

	// Attachment points
	vec3_t					attachment[4];

	// Other entity local information
	int						trivial_accept;

	model_t *model;			// cl.model_precache[ curstate.modelindes ];  all visible entities have a model
	efrag_t *efrag;			// linked list of efrags
	mnode_t *topnode;		// for bmodels, first world node that splits bmodel, or NULL if not split

	float					syncbase;		// for client-side animations -- used by obsolete alias animation system, remove?
	int						visframe;		// last frame this entity was found in an active leaf
	colorVec				cvFloorColor;
};
