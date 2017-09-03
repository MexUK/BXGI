#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"

enum bxgi::EDATLoaderEntryType
{
	DAT_LOADER_UNKNOWN,
	DAT_LOADER_IMG,
	DAT_LOADER_CDIMAGE,
	DAT_LOADER_IMGLIST,
	DAT_LOADER_WATER,
	DAT_LOADER_IDE,
	DAT_LOADER_HIERFILE,
	DAT_LOADER_COLFILE,
	DAT_LOADER_IPL,
	DAT_LOADER_MAPZONE,
	DAT_LOADER_TEXDICTION,
	DAT_LOADER_MODELFILE,
	DAT_LOADER_SPLASH,
	DAT_LOADER_EXIT
};