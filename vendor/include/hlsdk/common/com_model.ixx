export module com_model;

export import const_;
export import custom;
export import vector;

export inline constexpr auto STUDIO_RENDER = 1;
export inline constexpr auto STUDIO_EVENTS = 2;

export inline constexpr auto MAX_CLIENTS = 32;
export inline constexpr auto MAX_EDICTS = 900;

export inline constexpr auto MAX_MODEL_NAME = 64;
export inline constexpr auto MAX_MAP_HULLS = 4;
export inline constexpr auto MIPLEVELS = 4;
export inline constexpr auto NUM_AMBIENTS = 4;		// automatic ambient sounds
export inline constexpr auto MAXLIGHTMAPS = 4;
export inline constexpr auto PLANE_ANYZ = 5;

export inline constexpr auto ALIAS_Z_CLIP_PLANE = 5;

// flags in finalvert_t.flags
export inline constexpr auto ALIAS_LEFT_CLIP = 0x0001;
export inline constexpr auto ALIAS_TOP_CLIP = 0x0002;
export inline constexpr auto ALIAS_RIGHT_CLIP = 0x0004;
export inline constexpr auto ALIAS_BOTTOM_CLIP = 0x0008;
export inline constexpr auto ALIAS_Z_CLIP = 0x0010;
export inline constexpr auto ALIAS_ONSEAM = 0x0020;
export inline constexpr auto ALIAS_XY_CLIP_MASK = 0x000F;

export inline constexpr auto ZISCALE = ((float)0x8000);

export inline constexpr auto CACHE_SIZE = 32;		// used to align key data structures

export enum modtype_t
{
	mod_brush,
	mod_sprite,
	mod_alias,
	mod_studio
};

// must match definition in modelgen.h
export enum synctype_t
{
	ST_SYNC = 0,
	ST_RAND
};

export struct dmodel_t
{
	float		mins[3], maxs[3];
	float		origin[3];
	int			headnode[MAX_MAP_HULLS];
	int			visleafs;		// not including the solid leaf 0
	int			firstface, numfaces;
};

// plane_t structure
export struct mplane_t
{
	vec3_t	normal;			// surface normal
	float	dist;			// closest appoach to origin
	byte	type;			// for texture axis selection and fast side tests
	byte	signbits;		// signx + signy<<1 + signz<<1
	byte	pad[2];
};

export struct mvertex_t
{
	vec3_t		position;
};

export struct medge_t
{
	unsigned short	v[2];
	unsigned int	cachededgeoffset;
};

export struct texture_t
{
	char		name[16];
	unsigned	width, height;
	int			anim_total;				// total tenths in sequence ( 0 = no)
	int			anim_min, anim_max;		// time for this frame min <=time< max
	texture_t	*anim_next;		// in the animation sequence
	texture_t	*alternate_anims;	// bmodels in frame 1 use these
	unsigned	offsets[MIPLEVELS];		// four mip maps stored
	unsigned	paloffset;
};

export struct mtexinfo_t
{
	float		vecs[2][4];		// [s/t] unit vectors in world space. 
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	float		mipadjust;		// ?? mipmap limits for very small surfaces
	texture_t	*texture;
	int			flags;			// sky or slime, no lightmap or 256 subdivision
};

struct msurface_t;

// JAY: Compress this as much as possible
export struct decal_t
{
	decal_t *pnext;			// linked list for each surface
	msurface_t *psurface;		// Surface id for persistence / unlinking
	short		dx;				// Offsets into surface texture (in texture coordinates, so we don't need floats)
	short		dy;
	short		texture;		// Decal texture
	byte		scale;			// Pixel scale
	byte		flags;			// Decal flags

	short		entityIndex;	// Entity this is attached to
};

struct efrag_t;	// check cl_entity.ixx

export struct mnode_t
{
	// common with leaf
	int			contents;		// 0, to differentiate from leafs
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	mnode_t*	parent;

	// node specific
	mplane_t*	plane;
	mnode_t*	children[2];

	unsigned short	firstsurface;
	unsigned short	numsurfaces;
};

export struct mleaf_t
{
	// common with node
	int				contents;		// wil be a negative contents number
	int				visframe;		// node needs to be traversed if current

	short			minmaxs[6];		// for bounding box culling

	mnode_t*		parent;

	// leaf specific
	byte*			compressed_vis;
	efrag_t*		efrags;

	msurface_t**	firstmarksurface;
	int				nummarksurfaces;
	int				key;			// BSP sequence number for leaf's contents
	byte			ambient_sound_level[NUM_AMBIENTS];
};

struct texture_t;	// #SHOULD_DO_ON_FREE

export struct surfcache_t
{
	surfcache_t*	next;
	surfcache_t**	owner;
	int				lightadj[4];
	int				dlight;
	int				size;
	unsigned		width;
	unsigned		height;
	float			mipscale;
	texture_t*		texture;
	unsigned char	data[4];
};

export struct msurface_t
{
	int			visframe;		// should be drawn when node is crossed

	int			dlightframe;	// last frame the surface was checked by an animated light
	int			dlightbits;		// dynamically generated. Indicates if the surface illumination 
	// is modified by an animated light.

	mplane_t	*plane;			// pointer to shared plane			
	int			flags;			// see SURF_ #defines

	int			firstedge;	// look up in model->surfedges[], negative numbers
	int			numedges;	// are backwards edges

	// surface generation data
	surfcache_t	*cachespots[MIPLEVELS];

	short		texturemins[2]; // smallest s/t position on the surface.
	short		extents[2];		// ?? s/t texture size, 1..256 for all non-sky surfaces

	mtexinfo_t	*texinfo;

	// lighting info
	byte		styles[MAXLIGHTMAPS]; // index into d_lightstylevalue[] for animated lights 
	// no one surface can be effected by more than 4 
	// animated lights.
	color24		*samples;

	decal_t		*pdecals;
};

export struct dclipnode_t
{
	int			planenum;
	short		children[2];	// negative numbers are contents
};

export struct hull_t
{
	dclipnode_t	*clipnodes;
	mplane_t	*planes;
	int			firstclipnode;
	int			lastclipnode;
	vec3_t		clip_mins;
	vec3_t		clip_maxs;
};

export struct cache_user_t
{
	void *data;
};

export struct model_t
{
	char		name[MAX_MODEL_NAME];
	qboolean	needload;		// bmodels and sprites don't cache normally

	modtype_t	type;
	int			numframes;
	synctype_t	synctype;

	int			flags;

	//
	// volume occupied by the model
	//		
	vec3_t		mins, maxs;
	float		radius;

	//
	// brush model
	//
	int			firstmodelsurface, nummodelsurfaces;

	int			numsubmodels;
	dmodel_t	*submodels;

	int			numplanes;
	mplane_t	*planes;

	int			numleafs;		// number of visible leafs, not counting 0
	mleaf_t		*leafs;

	int			numvertexes;
	mvertex_t	*vertexes;

	int			numedges;
	medge_t		*edges;

	int			numnodes;
	mnode_t		*nodes;

	int			numtexinfo;
	mtexinfo_t	*texinfo;

	int			numsurfaces;
	msurface_t	*surfaces;

	int			numsurfedges;
	int			*surfedges;

	int			numclipnodes;
	dclipnode_t *clipnodes;

	int			nummarksurfaces;
	msurface_t	**marksurfaces;

	hull_t		hulls[MAX_MAP_HULLS];

	int			numtextures;
	texture_t	**textures;

	byte		*visdata;

	color24		*lightdata;

	char		*entities;

	//
	// additional model data
	//
	cache_user_t	cache;		// only access through Mod_Extradata

};

export struct alight_t
{
	int			ambientlight;	// clip at 128
	int			shadelight;		// clip at 192 - ambientlight
	vec3_t		color;
	float		*plightvec;
};

export struct auxvert_t
{
	float	fv[3];		// viewspace x, y
};


export inline constexpr auto MAX_INFO_STRING = 256;
export inline constexpr auto MAX_SCOREBOARDNAME = 32;

export struct player_info_t
{
	// User id on server
	int		userid;

	// User info string
	char	userinfo[MAX_INFO_STRING];

	// Name
	char	name[MAX_SCOREBOARDNAME];

	// Spectator or not, unused
	int		spectator;

	int		ping;
	int		packet_loss;

	// skin information
	char	model[MAX_QPATH];
	int		topcolor;
	int		bottomcolor;

	// last frame rendered
	int		renderframe;

	// Gait frame estimation
	int		gaitsequence;
	float	gaitframe;
	float	gaityaw;
	vec3_t	prevgaitorigin;

	customization_t customdata;
};
