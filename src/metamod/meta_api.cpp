#include "precompiled.h"
#include "dllmain.h"

meta_globals_t* gpMetaGlobals;		// metamod globals
gamedll_funcs_t* gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t* gpMetaUtilFuncs;		// metamod utility functions

plugin_info_t Plugin_info =
{
	.ifvers		= META_INTERFACE_VERSION,
	.name		= MODULE_NAME,
	.version	= MODULE_VERSION,
	.date		= MODULE_DATE,
	.author		= MODULE_AUTHOR,
	.url		= MODULE_URL,
	.logtag		= MODULE_LOGTAG,
	.loadable	= PT_ANYTIME,
	.unloadable = PT_ANYPAUSE,
};

// Must provide at least one of these..
inline constexpr META_FUNCTIONS gMetaFunctionTable =
{
	.pfnGetEntityAPI					= nullptr,				// HL SDK; called before game DLL
	.pfnGetEntityAPI_Post				= nullptr,				// META; called after game DLL
	.pfnGetEntityAPI2					= &GetEntityAPI2_Post,	// HL SDK2; called before game DLL
	.pfnGetEntityAPI2_Post				= nullptr,				// META; called after game DLL
	.pfnGetNewDLLFunctions				= nullptr,				// HL SDK2; called before game DLL
	.pfnGetNewDLLFunctions_Post			= nullptr,				// META; called after game DLL
	.pfnGetEngineFunctions				= &GetEngineFunctions,	// META; called before HL engine
	.pfnGetEngineFunctions_Post			= nullptr,				// META; called after HL engine
};

// Metamod requesting info about this plugin:
//  ifvers			(given) interface_version metamod is using
//  pPlugInfo		(requested) struct with info about plugin
//  pMetaUtilFuncs	(given) table of utility functions provided by metamod
int Meta_Query(const char* pszInterfaceVersion, plugin_info_t const** pPlugInfo, mutil_funcs_t* pMetaUtilFuncs) noexcept
{
	*pPlugInfo = PLID;
	gpMetaUtilFuncs = pMetaUtilFuncs;

	return true;
}
static_assert(std::same_as<decltype(&Meta_Query), META_QUERY_FN>);

// Metamod attaching plugin to the server.
//  now				(given) current phase, ie during map, during changelevel, or at startup
//  pFunctionTable	(requested) table of function tables this plugin catches
//  pMGlobals		(given) global vars from metamod
//  pGamedllFuncs	(given) copy of function tables from game dll
int Meta_Attach(PLUG_LOADTIME iCurrentPhase, META_FUNCTIONS* pFunctionTable, meta_globals_t* pMGlobals, gamedll_funcs_t* pGamedllFuncs) noexcept
{
	if (!pMGlobals)
	{
		gpMetaUtilFuncs->pfnLogError(PLID, "Function 'Meta_Attach' called with null 'pMGlobals' parameter.");
		return false;
	}

	gpMetaGlobals = pMGlobals;

	if (!pFunctionTable)
	{
		gpMetaUtilFuncs->pfnLogError(PLID, "Function 'Meta_Attach' called with null 'pFunctionTable' parameter.");
		return false;
	}

	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
	gpGamedllFuncs = pGamedllFuncs;

	OnMetaAttach(iCurrentPhase);

	return true;
}
static_assert(std::same_as<decltype(&Meta_Attach), META_ATTACH_FN>);

// Metamod detaching plugin from the server.
// now		(given) current phase, ie during map, etc
// reason	(given) why detaching (refresh, console unload, forced unload, etc)
int Meta_Detach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason) noexcept
{
	OnMetaDetach(iCurrentPhase, iReason);

	return true;
}
static_assert(std::same_as<decltype(&Meta_Detach), META_DETACH_FN>);