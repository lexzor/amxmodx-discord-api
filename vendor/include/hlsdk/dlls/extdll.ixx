export module extdll;

// Prevent tons of unused windows definitions
//#define WIN32_LEAN_AND_MEAN
//#define NOWINRES
//#define NOSERVICE
//#define NOMCX
//#define NOIME
//export import <Windows.h>;

// Header file containing definition of globalvars_t and entvars_t
//typedef int	func_t;					//
//typedef int	string_t;				// from engine's pr_comp.h;
//typedef float vec_t;				// needed before including progdefs.h

// Vector class
export import vector;

// Defining it as a (bogus) struct helps enforce type-checking
//#define vec3_t Vector

// Shared engine/DLL constants
export import const_;
export import progdefs;
export import edict;

// Shared header describing protocol between engine and DLLs
export import eiface;

// Shared header between the client DLL and the game DLLs
export import cdll_dll;
