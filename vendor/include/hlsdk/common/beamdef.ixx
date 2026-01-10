module;

#include <cstdint>

export module beamdef;

export import com_model;
export import particledef;
export import vector;

export inline constexpr std::uint32_t FBEAM_STARTENTITY	= 0x00000001;
export inline constexpr std::uint32_t FBEAM_ENDENTITY = 0x00000002;
export inline constexpr std::uint32_t FBEAM_FADEIN = 0x00000004;
export inline constexpr std::uint32_t FBEAM_FADEOUT = 0x00000008;
export inline constexpr std::uint32_t FBEAM_SINENOISE = 0x00000010;
export inline constexpr std::uint32_t FBEAM_SOLID = 0x00000020;
export inline constexpr std::uint32_t FBEAM_SHADEIN = 0x00000040;
export inline constexpr std::uint32_t FBEAM_SHADEOUT = 0x00000080;
export inline constexpr std::uint32_t FBEAM_STARTVISIBLE = 0x10000000;		// Has this client actually seen this beam's start entity yet?
export inline constexpr std::uint32_t FBEAM_ENDVISIBLE = 0x20000000;		// Has this client actually seen this beam's end entity yet?
export inline constexpr std::uint32_t FBEAM_ISACTIVE = 0x40000000;
export inline constexpr std::uint32_t FBEAM_FOREVER = 0x80000000;

export struct beam_s
{
	beam_s		*next;
	int			type;
	int			flags;
	vec3_t		source;
	vec3_t		target;
	vec3_t		delta;
	float		t;		// 0 .. 1 over lifetime of beam
	float		freq;
	float		die;
	float		width;
	float		amplitude;
	float		r, g, b;
	float		brightness;
	float		speed;
	float		frameRate;
	float		frame;
	int			segments;
	int			startEntity;
	int			endEntity;
	int			modelIndex;
	int			frameCount;
	model_t		*pFollowModel;
	particle_t	*particles;
};

export using BEAM = beam_s;
