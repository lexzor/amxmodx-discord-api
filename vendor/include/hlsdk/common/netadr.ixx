export module netadr;

export enum netadrtype_t
{
	NA_UNUSED,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX,
};

export struct netadr_s
{
	netadrtype_t	type;
	unsigned char	ip[4];
	unsigned char	ipx[10];
	unsigned short	port;
};

export using netadr_t = netadr_s;
