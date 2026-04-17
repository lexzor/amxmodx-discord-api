#include "module.h"

cell AMX_NATIVE_CALL GetModuleVersionString(AMX* amx, cell* params)
{
    char* buffer = MF_GetAmxString(amx, params[1], 0, nullptr);
    uint32_t bufferLen = static_cast<uint32_t>(params[2]);

    const uint8_t versionStrLen = strlen(MODULE_VERSION);

    if(versionStrLen > bufferLen)
    {
        MF_LogError(amx, AMX_ERR_BOUNDS, "index out of bounds");
        return -1;
    }

    memset(buffer, 0x00, bufferLen);
    strncpy(buffer, MODULE_VERSION, versionStrLen);
    buffer[versionStrLen] = 0x0;

    return versionStrLen;
}
