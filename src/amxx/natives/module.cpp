#include "module.h"

cell AMX_NATIVE_CALL GetModuleVersionString(AMX* amx, cell* params)
{
    if (params[0] < 2)
        return 0;

    size_t versionStrLen = strlen(MODULE_VERSION);

    // params[2] = buffer size
    if (versionStrLen >= static_cast<size_t>(params[2]))
    {
        MF_LogError(amx, AMX_ERR_BOUNDS, "index out of bounds");
        return -1;
    }

    MF_SetAmxString(amx, params[1], MODULE_VERSION, params[2]);

    return static_cast<cell>(versionStrLen);
}