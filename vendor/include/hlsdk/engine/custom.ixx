export module custom;

export import const_;

export inline constexpr auto MAX_QPATH = 64;    // Must match value in quakedefs.h

/////////////////
// Customization
// passed to pfnPlayerCustomization
// For automatic downloading.
export enum resourcetype_t
{
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript
};

// Fake type for world
export inline constexpr auto t_world = 6;

export struct resourceinfo_s
{
	struct { int size; } info[7];
};

export using resourceinfo_t = resourceinfo_s;

export inline constexpr auto RES_FATALIFMISSING = (1 << 0);   // Disconnect if we can't get this file.
export inline constexpr auto RES_WASMISSING = (1 << 1);   // Do we have the file locally, did we get it ok?
export inline constexpr auto RES_CUSTOM = (1 << 2);   // Is this resource one that corresponds to another player's customization or is it a server startup resource.
export inline constexpr auto RES_REQUESTED = (1 << 3);	// Already requested a download of this one
export inline constexpr auto RES_PRECACHED = (1 << 4);	// Already precached

export struct resource_s
{
	char              szFileName[MAX_QPATH]; // File name to download/precache.
	resourcetype_t    type;                // t_sound, t_skin, t_model, t_decal.
	int               nIndex;              // For t_decals
	int               nDownloadSize;       // Size in Bytes if this must be downloaded.
	unsigned char     ucFlags;

	// For handling client to client resource propagation
	unsigned char     rgucMD5_hash[16];    // To determine if we already have it.
	unsigned char     playernum;           // Which player index this resource is associated with, if it's a custom resource.

	unsigned char	  rguc_reserved[32]; // For future expansion
	struct resource_s *pNext;              // Next in chain.
	struct resource_s *pPrev;
};

export using resource_t = resource_s;

export struct customization_s
{
	qboolean bInUse;     // Is this customization in use;
	resource_t resource; // The resource_t for this customization
	qboolean bTranslated; // Has the raw data been translated into a useable format?  
	//  (e.g., raw decal .wad make into texture_t *)
	int nUserData1; // Customization specific data
	int nUserData2; // Customization specific data
	void *pInfo;          // Buffer that holds the data structure that references the data (e.g., the cachewad_t)
	void *pBuffer;       // Buffer that holds the data for the customization (the raw .wad data)
	struct customization_s *pNext; // Next in chain
};

export using customization_t = customization_s;

export inline constexpr auto FCUST_FROMHPAK = (1 << 0);
export inline constexpr auto FCUST_WIPEDATA = (1 << 1);
export inline constexpr auto FCUST_IGNOREINIT = (1 << 2);

export void	COM_ClearCustomizationList(customization_s *pHead, qboolean bCleanDecals);
export qboolean	COM_CreateCustomization(customization_s *pListHead, resource_s *pResource, int playernumber, int flags, customization_s **pCustomization, int *nLumps);
export int COM_SizeofResourceList(resource_s *pList, resourceinfo_s *ri);
