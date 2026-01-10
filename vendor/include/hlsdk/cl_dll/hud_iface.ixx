export module hud_iface;

export using pfnUserMsgHook = int (*)(const char *pszName, int iSize, void *pbuf);
