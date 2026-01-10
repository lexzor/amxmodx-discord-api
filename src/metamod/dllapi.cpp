#include "precompiled.h"

DLL_FUNCTIONS gFunctionTable =
{
	.pfnGameInit = nullptr,
	.pfnSpawn = nullptr,
	.pfnThink = nullptr,
	.pfnUse = nullptr,
	.pfnTouch = nullptr,
	.pfnBlocked = nullptr,
	.pfnKeyValue = nullptr,
	.pfnSave = nullptr,
	.pfnRestore = nullptr,
	.pfnSetAbsBox = nullptr,

	.pfnSaveWriteFields = nullptr,
	.pfnSaveReadFields = nullptr,

	.pfnSaveGlobalState = nullptr,
	.pfnRestoreGlobalState = nullptr,
	.pfnResetGlobalState = nullptr,

	.pfnClientConnect = nullptr,
	.pfnClientDisconnect = nullptr,
	.pfnClientKill = nullptr,
	.pfnClientPutInServer = nullptr,
	.pfnClientCommand = nullptr,
	.pfnClientUserInfoChanged = nullptr,
	.pfnServerActivate = nullptr,
	.pfnServerDeactivate = nullptr,

	.pfnPlayerPreThink = nullptr,
	.pfnPlayerPostThink = nullptr,

	.pfnStartFrame = nullptr,
	.pfnParmsNewLevel = nullptr,
	.pfnParmsChangeLevel = nullptr,

	.pfnGetGameDescription = nullptr,
	.pfnPlayerCustomization = nullptr,

	.pfnSpectatorConnect = nullptr,
	.pfnSpectatorDisconnect = nullptr,
	.pfnSpectatorThink = nullptr,

	.pfnSys_Error = nullptr,

	.pfnPM_Move = nullptr,
	.pfnPM_Init = nullptr,
	.pfnPM_FindTextureType = nullptr,

	.pfnSetupVisibility = nullptr,
	.pfnUpdateClientData = nullptr,
	.pfnAddToFullPack = nullptr,
	.pfnCreateBaseline = nullptr,
	.pfnRegisterEncoders = nullptr,
	.pfnGetWeaponData = nullptr,
	.pfnCmdStart = nullptr,
	.pfnCmdEnd = nullptr,
	.pfnConnectionlessPacket = nullptr,
	.pfnGetHullBounds = nullptr,
	.pfnCreateInstancedBaselines = nullptr,
	.pfnInconsistentFile = nullptr,
	.pfnAllowLagCompensation = nullptr,
};

int GetEntityAPI2_Post(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	if (!pFunctionTable) [[unlikely]]
	{
		gpMetaUtilFuncs->pfnLogError(PLID, "Function 'HookGameDLLExportedFn' called with null 'pFunctionTable' parameter.");
		return false;
	}
	else if (*interfaceVersion != INTERFACE_VERSION) [[unlikely]]
	{
		gpMetaUtilFuncs->pfnLogError(PLID, "Function 'HookGameDLLExportedFn' called with version mismatch. Expected: %d, receiving: %d.", INTERFACE_VERSION, *interfaceVersion);

		//! Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return false;
	}

	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return true;
}