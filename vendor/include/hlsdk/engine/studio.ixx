export module studio;

export import <numbers>;

export import vector;
export import com_model;


/*
==============================================================================

STUDIO MODELS

Studio models are position independent, so the cache manager can move them.
==============================================================================
*/

export inline constexpr auto MAXSTUDIOTRIANGLES = 20000;	// TODO: tune this
export inline constexpr auto MAXSTUDIOVERTS = 2048;	// TODO: tune this
export inline constexpr auto MAXSTUDIOSEQUENCES = 256;		// total animation sequences
export inline constexpr auto MAXSTUDIOSKINS = 100;		// total textures
export inline constexpr auto MAXSTUDIOSRCBONES = 512;		// bones allowed at source movement
export inline constexpr auto MAXSTUDIOBONES = 128;		// total bones actually used
export inline constexpr auto MAXSTUDIOMODELS = 32;		// sub-models per model
export inline constexpr auto MAXSTUDIOBODYPARTS = 32;
export inline constexpr auto MAXSTUDIOGROUPS = 16;
export inline constexpr auto MAXSTUDIOANIMATIONS = 512;		// per sequence
export inline constexpr auto MAXSTUDIOMESHES = 256;
export inline constexpr auto MAXSTUDIOEVENTS = 1024;
export inline constexpr auto MAXSTUDIOPIVOTS = 256;
export inline constexpr auto MAXSTUDIOCONTROLLERS = 8;

export struct studiohdr_t
{
	int					id;
	int					version;

	char				name[64];
	int					length;

	vec3_t				eyeposition;	// ideal eye position
	vec3_t				min;			// ideal movement hull size
	vec3_t				max;

	vec3_t				bbmin;			// clipping bounding box
	vec3_t				bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxes;			// complex bounding boxes
	int					hitboxindex;

	int					numseq;				// animation sequences
	int					seqindex;

	int					numseqgroups;		// demand loaded sequences
	int					seqgroupindex;

	int					numtextures;		// raw textures
	int					textureindex;
	int					texturedataindex;

	int					numskinref;			// replaceable textures
	int					numskinfamilies;
	int					skinindex;

	int					numbodyparts;
	int					bodypartindex;

	int					numattachments;		// queryable attachable points
	int					attachmentindex;

	int					soundtable;
	int					soundindex;
	int					soundgroups;
	int					soundgroupindex;

	int					numtransitions;		// animation node to animation node transition graph
	int					transitionindex;
};

// header for demand loaded sequence group data
export struct studioseqhdr_t
{
	int					id;
	int					version;

	char				name[64];
	int					length;
};

// bones
export struct mstudiobone_t
{
	char				name[32];	// bone name for symbolic links
	int		 			parent;		// parent bone
	int					flags;		// ??
	int					bonecontroller[6];	// bone controller index, -1 == none
	float				value[6];	// default DoF values
	float				scale[6];   // scale for delta DoF values
};


// bone controllers
export struct mstudiobonecontroller_t
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					index;	// 0-3 user set controller, 4 mouth
};

// intersection boxes
export struct mstudiobbox_t
{
	int					bone;
	int					group;			// intersection group
	vec3_t				bbmin;		// bounding box
	vec3_t				bbmax;
};


// demand loaded sequence groups
export struct mstudioseqgroup_t
{
	char				label[32];	// textual name
	char				name[64];	// file name
	cache_user_t		cache;		// cache index pointer
	int					data;		// hack for group 0
};

// sequence descriptions
export struct mstudioseqdesc_t
{
	char				label[32];	// sequence label

	float				fps;		// frames per second	
	int					flags;		// looping/non-looping flags

	int					activity;
	int					actweight;

	int					numevents;
	int					eventindex;

	int					numframes;	// number of frames per sequence

	int					numpivots;	// number of foot pivots
	int					pivotindex;

	int					motiontype;
	int					motionbone;
	vec3_t				linearmovement;
	int					automoveposindex;
	int					automoveangleindex;

	vec3_t				bbmin;		// per sequence bounding box
	vec3_t				bbmax;

	int					numblends;
	int					animindex;		// mstudioanim_t pointer relative to start of sequence group data
	// [blend][bone][X, Y, Z, XR, YR, ZR]

	int					blendtype[2];	// X, Y, Z, XR, YR, ZR
	float				blendstart[2];	// starting value
	float				blendend[2];	// ending value
	int					blendparent;

	int					seqgroup;		// sequence group for demand loading

	int					entrynode;		// transition node at entry
	int					exitnode;		// transition node at exit
	int					nodeflags;		// transition rules

	int					nextseq;		// auto advancing sequences
};

// events
export import studio_event;
/*
typedef struct
{
	int 				frame;
	int					event;
	int					type;
	char				options[64];
} mstudioevent_t;
*/

// pivots
export struct mstudiopivot_t
{
	vec3_t				org;	// pivot point
	int					start;
	int					end;
};

// attachment
export struct mstudioattachment_t
{
	char				name[32];
	int					type;
	int					bone;
	vec3_t				org;	// attachment point
	vec3_t				vectors[3];
};

export struct mstudioanim_t
{
	unsigned short	offset[6];
};

// animation frames
export union mstudioanimvalue_t
{
	struct {
		byte	valid;
		byte	total;
	} num;
	short		value;
};



// body part index
export struct mstudiobodyparts_t
{
	char				name[64];
	int					nummodels;
	int					base;
	int					modelindex; // index into models array
};



// skin info
export struct mstudiotexture_t
{
	char					name[64];
	int						flags;
	int						width;
	int						height;
	int						index;
};


// skin families
// short	index[skinfamilies][skinref]

// studio models
export struct mstudiomodel_t
{
	char				name[64];

	int					type;

	float				boundingradius;

	int					nummesh;
	int					meshindex;

	int					numverts;		// number of unique vertices
	int					vertinfoindex;	// vertex bone info
	int					vertindex;		// vertex vec3_t
	int					numnorms;		// number of unique surface normals
	int					norminfoindex;	// normal bone info
	int					normindex;		// normal vec3_t

	int					numgroups;		// deformation groups
	int					groupindex;
};


// vec3_t	boundingbox[model][bone][2];	// complex intersection info


// meshes
export struct mstudiomesh_t
{
	int					numtris;
	int					triindex;
	int					skinref;
	int					numnorms;		// per mesh normals
	int					normindex;		// normal vec3_t
};

// triangles
#if 0
export struct mstudiotrivert_t
{
	short				vertindex;		// index into vertex array
	short				normindex;		// index into normal array
	short				s, t;			// s,t position on skin
};
#endif

// lighting options
export inline constexpr auto STUDIO_NF_FLATSHADE = 0x0001;
export inline constexpr auto STUDIO_NF_CHROME = 0x0002;
export inline constexpr auto STUDIO_NF_FULLBRIGHT = 0x0004;

// motion flags
export inline constexpr auto STUDIO_X = 0x0001;
export inline constexpr auto STUDIO_Y = 0x0002;
export inline constexpr auto STUDIO_Z = 0x0004;
export inline constexpr auto STUDIO_XR = 0x0008;
export inline constexpr auto STUDIO_YR = 0x0010;
export inline constexpr auto STUDIO_ZR = 0x0020;
export inline constexpr auto STUDIO_LX = 0x0040;
export inline constexpr auto STUDIO_LY = 0x0080;
export inline constexpr auto STUDIO_LZ = 0x0100;
export inline constexpr auto STUDIO_AX = 0x0200;
export inline constexpr auto STUDIO_AY = 0x0400;
export inline constexpr auto STUDIO_AZ = 0x0800;
export inline constexpr auto STUDIO_AXR = 0x1000;
export inline constexpr auto STUDIO_AYR = 0x2000;
export inline constexpr auto STUDIO_AZR = 0x4000;
export inline constexpr auto STUDIO_TYPES = 0x7FFF;
export inline constexpr auto STUDIO_RLOOP = 0x8000;	// controller that wraps shortest distance

// sequence flags
export inline constexpr auto STUDIO_LOOPING = 0x0001;

// bone flags
export inline constexpr auto STUDIO_HAS_NORMALS = 0x0001;
export inline constexpr auto STUDIO_HAS_VERTICES = 0x0002;
export inline constexpr auto STUDIO_HAS_BBOX = 0x0004;
export inline constexpr auto STUDIO_HAS_CHROME = 0x0008;	// if any of the textures have chrome on them

export inline constexpr auto RAD_TO_STUDIO = (32768.0 / std::numbers::pi);
export inline constexpr auto STUDIO_TO_RAD = (std::numbers::pi / 32768.0);

static_assert(STUDIO_TO_RAD != 0);
