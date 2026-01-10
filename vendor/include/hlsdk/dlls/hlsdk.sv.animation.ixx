export module hlsdk:sv.animation;

#ifdef EXPORT
#undef EXPORT
#endif

#ifdef INCLUDED_IN_MODULE
#undef INCLUDED_IN_MODULE
#endif

#define EXPORT export
#define INCLUDED_IN_MODULE

extern "C++"
{
#include "hlsdk.sv.animation.hpp"
}
