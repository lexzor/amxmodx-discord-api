export module qfont;

export import const_;

// Font stuff

export inline constexpr auto NUM_GLYPHS = 256;

export struct charinfo
{
	short startoffset;
	short charwidth;
};

export struct qfont_s
{
	int 		width, height;
	int			rowcount;
	int			rowheight;
	charinfo	fontinfo[NUM_GLYPHS];
	byte 		data[4];
};

export using qfont_t = qfont_s;