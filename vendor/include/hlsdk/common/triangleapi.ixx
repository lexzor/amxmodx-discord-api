export module triangleapi;

export import com_model;

export enum TRICULLSTYLE
{
	TRI_FRONT = 0,
	TRI_NONE = 1,
};

export inline constexpr auto TRI_API_VERSION = 1;

export inline constexpr auto TRI_TRIANGLES = 0;
export inline constexpr auto TRI_TRIANGLE_FAN = 1;
export inline constexpr auto TRI_QUADS = 2;
export inline constexpr auto TRI_POLYGON = 3;
export inline constexpr auto TRI_LINES = 4;
export inline constexpr auto TRI_TRIANGLE_STRIP = 5;
export inline constexpr auto TRI_QUAD_STRIP = 6;

export struct triangleapi_s
{
	int			version;

	void		(*RenderMode)(int mode);
	void		(*Begin)(int primitiveCode);
	void		(*End) (void);

	void		(*Color4f) (float r, float g, float b, float a);
	void		(*Color4ub) (unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void		(*TexCoord2f) (float u, float v);
	void		(*Vertex3fv) (float *worldPnt);
	void		(*Vertex3f) (float x, float y, float z);
	void		(*Brightness) (float brightness);
	void		(*CullFace) (TRICULLSTYLE style);
	int			(*SpriteTexture) (model_t *pSpriteModel, int frame);
	int			(*WorldToScreen) (float *world, float *screen);  // Returns 1 if it's z clipped
	void		(*Fog) (float flFogColor[3], float flStart, float flEnd, int bOn); //Works just like GL_FOG, flFogColor is r/g/b.
	void		(*ScreenToWorld) (float *screen, float *world);

};

export using triangleapi_t = triangleapi_s;
