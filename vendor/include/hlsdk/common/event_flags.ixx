export module event_flags;

// Skip local host for event send.
export inline constexpr auto FEV_NOTHOST = (1 << 0);

// Send the event reliably.  You must specify the origin and angles and use
// PLAYBACK_EVENT_FULL for this to work correctly on the server for anything
// that depends on the event origin/angles.  I.e., the origin/angles are not
// taken from the invoking edict for reliable events.
export inline constexpr auto FEV_RELIABLE = (1 << 1);

// Don't restrict to PAS/PVS, send this event to _everybody_ on the server ( useful for stopping CHAN_STATIC
//  sounds started by client event when client is not in PVS anymore ( hwguy in TFC e.g. ).
export inline constexpr auto FEV_GLOBAL = (1 << 2);

// If this client already has one of these events in its queue, just update the event instead of sending it as a duplicate
export inline constexpr auto FEV_UPDATE = (1 << 3);

// Only send to entity specified as the invoker
export inline constexpr auto FEV_HOSTONLY = (1 << 4);

// Only send if the event was created on the server.
export inline constexpr auto FEV_SERVER = (1 << 5);

// Only issue event client side ( from shared code )
export inline constexpr auto FEV_CLIENT = (1 << 6);
