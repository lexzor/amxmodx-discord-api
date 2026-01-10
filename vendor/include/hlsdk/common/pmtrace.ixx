export module pmtrace;

export import const_;
export import vector;

export struct pmplane_t
{
	vec3_t	normal;
	float	dist;
};

export struct pmtrace_s
{
	qboolean	allsolid;	      // if true, plane is not valid
	qboolean	startsolid;	      // if true, the initial point was in a solid area
	qboolean	inopen, inwater;  // End point is in empty space or in water
	float		fraction;		  // time completed, 1.0 = didn't hit anything
	vec3_t		endpos;			  // final position
	pmplane_t	plane;		      // surface normal at impact
	int			ent;			  // entity at impact
	vec3_t      deltavelocity;    // Change in player's velocity caused by impact.  
	// Only run on server.
	int         hitgroup;
};

export using pmtrace_t = pmtrace_s;
