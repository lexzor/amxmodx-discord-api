export module cdll_int;

export import cl_entity;
export import con_nprint;
export import const_;
export import cvardef;
export import demo_api;
export import event_api;
export import hud_iface;
export import in_buttons;
export import ivoicetweak;
export import net_api;
export import pmtrace;
export import r_efx;
export import screenfade;
export import triangleapi;
export import wrect;

// this file is included by both the engine and the client-dll,
// so make sure engine declarations aren't done twice

export using HSPRITE = int;	// handle to a graphic #POTENTIAL_BUG name overlap with windowsdef

export inline constexpr auto SCRINFO_SCREENFLASH = 1;
export inline constexpr auto SCRINFO_STRETCHED = 2;

export struct SCREENINFO_s
{
	int		iSize;
	int		iWidth;
	int		iHeight;
	int		iFlags;
	int		iCharHeight;
	short	charWidths[256];
};

export using SCREENINFO = SCREENINFO_s;

export struct client_data_s
{
	// fields that cannot be modified  (ie. have no effect if changed)
	vec3_t origin;

	// fields that can be changed by the cldll
	vec3_t viewangles;
	int		iWeaponBits;
	float	fov;	// field of view
};

export using client_data_t = client_data_s;

export struct client_sprite_s
{
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	wrect_t rc;
};

export using client_sprite_t = client_sprite_s;

export struct client_textmessage_s
{
	int		effect;
	byte	r1, g1, b1, a1;		// 2 colors for effects
	byte	r2, g2, b2, a2;
	float	x;
	float	y;
	float	fadein;
	float	fadeout;
	float	holdtime;
	float	fxtime;
	const char *pName;
	const char *pMessage;
};

export using client_textmessage_t = client_textmessage_s;

export struct hud_player_info_s
{
	char *name;
	short ping;
	byte thisplayer;  // TRUE if this is the calling player

	// stuff that's unused at the moment,  but should be done
	byte spectator;
	byte packetloss;

	char *model;
	short topcolor;
	short bottomcolor;
};

export using hud_player_info_t = hud_player_info_s;


export struct cl_enginefuncs_s
{
	// sprite handlers
	HSPRITE						(*pfnSPR_Load)				(const char *szPicName);
	int							(*pfnSPR_Frames)			(HSPRITE hPic);
	int							(*pfnSPR_Height)			(HSPRITE hPic, int frame);
	int							(*pfnSPR_Width)				(HSPRITE hPic, int frame);
	void						(*pfnSPR_Set)				(HSPRITE hPic, int r, int g, int b);
	void						(*pfnSPR_Draw)				(int frame, int x, int y, const wrect_t *prc);
	void						(*pfnSPR_DrawHoles)			(int frame, int x, int y, const wrect_t *prc);
	void						(*pfnSPR_DrawAdditive)		(int frame, int x, int y, const wrect_t *prc);
	void						(*pfnSPR_EnableScissor)		(int x, int y, int width, int height);
	void						(*pfnSPR_DisableScissor)	(void);
	client_sprite_t *			(*pfnSPR_GetList)			(char *psz, int *piCount);

	// screen handlers
	void						(*pfnFillRGBA)			(int x, int y, int width, int height, int r, int g, int b, int a);
	int							(*pfnGetScreenInfo) 	(SCREENINFO *pscrinfo);
	void						(*pfnSetCrosshair)		(HSPRITE hspr, wrect_t rc, int r, int g, int b);

	// cvar handlers
	cvar_t *					(*pfnRegisterVariable)	(const char *szName, const char *szValue, int flags);
	float						(*pfnGetCvarFloat)		(const char *szName);
	const char *				(*pfnGetCvarString)		(const char *szName);

	// command handlers
	int							(*pfnAddCommand)		(const char *cmd_name, void (*function)(void));
	int							(*pfnHookUserMsg)		(const char *szMsgName, pfnUserMsgHook pfn);
	int							(*pfnServerCmd)			(const char *szCmdString);
	int							(*pfnClientCmd)			(const char *szCmdString);

	void						(*pfnGetPlayerInfo)		(int ent_num, hud_player_info_t *pinfo);

	// sound handlers
	void						(*pfnPlaySoundByName)	(char *szSound, float volume);
	void						(*pfnPlaySoundByIndex)	(int iSound, float volume);

	// vector helpers
	void						(*pfnAngleVectors)		(const float *vecAngles, float *forward, float *right, float *up);

	// text message system
	client_textmessage_t *		(*pfnTextMessageGet)		(const char *pName);
	int							(*pfnDrawCharacter)			(int x, int y, int number, int r, int g, int b);
	int							(*pfnDrawConsoleString)		(int x, int y, char *string);
	void						(*pfnDrawSetTextColor)		(float r, float g, float b);
	void						(*pfnDrawConsoleStringLen)	(const char *string, int *length, int *height);

	void						(*pfnConsolePrint)			(const char *string);
	void						(*pfnCenterPrint)			(const char *string);


	// Added for user input processing
	int							(*GetWindowCenterX)		(void);
	int							(*GetWindowCenterY)		(void);
	void						(*GetViewAngles)		(float *);
	void						(*SetViewAngles)		(float *);
	int							(*GetMaxClients)		(void);
	void						(*Cvar_SetValue)		(const char *cvar, float value);

	int       					(*Cmd_Argc)				(void);
	char *						(*Cmd_Argv)				(int arg);
	void						(*Con_Printf)			(const char *fmt, ...);
	void						(*Con_DPrintf)			(const char *fmt, ...);
	void						(*Con_NPrintf)			(int pos, const char *fmt, ...);
	void						(*Con_NXPrintf)			(con_nprint_t *info, const char *fmt, ...);

	const char *				(*PhysInfo_ValueForKey)	(const char *key);
	const char *				(*ServerInfo_ValueForKey)(const char *key);
	float						(*GetClientMaxspeed)	(void);
	int							(*CheckParm)			(const char *parm, char **ppnext);
	void						(*Key_Event)			(int key, int down);
	void						(*GetMousePosition)		(int *mx, int *my);
	int							(*IsNoClipping)			(void);

	cl_entity_t *				(*GetLocalPlayer)		(void);
	cl_entity_t *				(*GetViewModel)			(void);
	cl_entity_t *				(*GetEntityByIndex)		(int idx);

	float						(*GetClientTime)		(void);
	void						(*V_CalcShake)			(void);
	void						(*V_ApplyShake)			(float *origin, float *angles, float factor);

	int							(*PM_PointContents)		(float *point, int *truecontents);
	int							(*PM_WaterEntity)		(float *p);
	pmtrace_t *					(*PM_TraceLine)			(float *start, float *end, int flags, int usehull, int ignore_pe);

	model_t *					(*CL_LoadModel)			(const char *modelname, int *index);
	int							(*CL_CreateVisibleEntity)(int type, cl_entity_t *ent);

	const model_t *				(*GetSpritePointer)		(HSPRITE hSprite);
	void						(*pfnPlaySoundByNameAtLocation)(char *szSound, float volume, float *origin);

	unsigned short				(*pfnPrecacheEvent)		(int type, const char *psz);
	void						(*pfnPlaybackEvent)		(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	void						(*pfnWeaponAnim)		(int iAnim, int body);
	float						(*pfnRandomFloat)		(float flLow, float flHigh);
	long						(*pfnRandomLong)		(long lLow, long lHigh);
	void						(*pfnHookEvent)			(char *name, void (*pfnEvent)(event_args_t *args));
	int							(*Con_IsVisible)		(void);
	const char *				(*pfnGetGameDirectory)	(void);
	cvar_t *					(*pfnGetCvarPointer)	(const char *szName);
	const char *				(*Key_LookupBinding)	(const char *pBinding);
	const char *				(*pfnGetLevelName)		(void);
	void						(*pfnGetScreenFade)		(screenfade_t *fade);
	void						(*pfnSetScreenFade)		(screenfade_t *fade);
	void *						(*VGui_GetPanel)		(void);
	void						(*VGui_ViewportPaintBackground) (int extents[4]);

	byte *						(*COM_LoadFile)			(const char *path, int usehunk, int *pLength);
	char *						(*COM_ParseFile)		(char *data, const char *token);
	void						(*COM_FreeFile)			(void *buffer);

	triangleapi_t *pTriAPI;
	efx_api_t *pEfxAPI;
	event_api_t *pEventAPI;
	demo_api_t *pDemoAPI;
	net_api_t *pNetAPI;
	IVoiceTweak *pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
	int							(*IsSpectateOnly)		(void);
	model_t *					(*LoadMapSprite)		(const char *filename);

	// file search functions
	void						(*COM_AddAppDirectoryToSearchPath)	(const char *pszBaseDir, const char *appName);
	int							(*COM_ExpandFilename)				(const char *fileName, char *nameOutBuffer, int nameOutBufferSize);

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
	const char *				(*PlayerInfo_ValueForKey)	(int playerNum, const char *key);
	void						(*PlayerInfo_SetValueForKey)(const char *key, const char *value);

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
	qboolean					(*GetPlayerUniqueID)	(int iPlayer, char playerID[16]);

	// TrackerID access
	int							(*GetTrackerIDForPlayer)(int playerSlot);
	int							(*GetPlayerForTrackerID)(int trackerID);

	// Same as pfnServerCmd, but the message goes in the unreliable stream so it can't clog the net stream
	// (but it might not get there).
	int							(*pfnServerCmdUnreliable)(char *szCmdString);

	void						(*pfnGetMousePos)		(struct tagPOINT *ppt);	// #NO_URGENT requires <windef>
	void						(*pfnSetMousePos)		(int x, int y);
	void						(*pfnSetMouseEnable)	(qboolean fEnable);
};

export using cl_enginefunc_t = cl_enginefuncs_s;

export inline constexpr auto CLDLL_INTERFACE_VERSION = 7;

// #STUB_FN
//extern void ClientDLL_Init(void); // from cdll_int.c
//extern void ClientDLL_Shutdown(void);
//extern void ClientDLL_HudInit(void);
//extern void ClientDLL_HudVidInit(void);
//extern void	ClientDLL_UpdateClientData(void);
//extern void ClientDLL_Frame(double time);
//extern void ClientDLL_HudRedraw(int intermission);
//extern void ClientDLL_MoveClient(struct playermove_s *ppmove);
//extern void ClientDLL_ClientMoveInit(struct playermove_s *ppmove);
//extern char ClientDLL_ClientTextureType(char *name);
//
//extern void ClientDLL_CreateMove(float frametime, struct usercmd_s *cmd, int active);
//extern void ClientDLL_ActivateMouse(void);
//extern void ClientDLL_DeactivateMouse(void);
//extern void ClientDLL_MouseEvent(int mstate);
//extern void ClientDLL_ClearStates(void);
//extern int ClientDLL_IsThirdPerson(void);
//extern void ClientDLL_GetCameraOffsets(float *ofs);
//extern int ClientDLL_GraphKeyDown(void);
//extern struct kbutton_s *ClientDLL_FindKey(const char *name);
//extern void ClientDLL_CAM_Think(void);
//extern void ClientDLL_IN_Accumulate(void);
//extern void ClientDLL_CalcRefdef(struct ref_params_s *pparams);
//extern int ClientDLL_AddEntity(int type, struct cl_entity_s *ent);
//extern void ClientDLL_CreateEntities(void);
//
//extern void ClientDLL_DrawNormalTriangles(void);
//extern void ClientDLL_DrawTransparentTriangles(void);
//extern void ClientDLL_StudioEvent(const struct mstudioevent_s *event, const struct cl_entity_s *entity);
//extern void ClientDLL_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
//extern void ClientDLL_TxferLocalOverrides(struct entity_state_s *state, const struct clientdata_s *client);
//extern void ClientDLL_ProcessPlayerState(struct entity_state_s *dst, const struct entity_state_s *src);
//extern void ClientDLL_TxferPredictionData(struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd);
//extern void ClientDLL_ReadDemoBuffer(int size, unsigned char *buffer);
//extern int ClientDLL_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
//extern int ClientDLL_GetHullBounds(int hullnumber, float *mins, float *maxs);
//
//extern void ClientDLL_VGui_ConsolePrint(const char *text);
//
//extern int ClientDLL_Key_Event(int down, int keynum, const char *pszCurrentBinding);
//extern void ClientDLL_TempEntUpdate(double ft, double ct, double grav, struct tempent_s **ppFreeTE, struct tempent_s **ppActiveTE, int (*addTEntity)(struct cl_entity_s *pEntity), void (*playTESound)(struct tempent_s *pTemp, float damp));
//extern struct cl_entity_s *ClientDLL_GetUserEntity(int index);
//extern void ClientDLL_VoiceStatus(int entindex, qboolean bTalking);
//extern void ClientDLL_DirectorMessage(int iSize, void *pbuf);
