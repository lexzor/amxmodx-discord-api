#pragma once

#include "precompiled.h"
#include <stdlib.h>     // Provides setenv and getenv
#include <filesystem>
#include <array>

void OnMetaAttach(PLUG_LOADTIME current_phase);
void OnMetaDetach(PLUG_LOADTIME current_phase, PL_UNLOAD_REASON reason);

void SetupOpenSSLCertPaths();