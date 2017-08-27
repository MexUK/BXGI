#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"

enum bxgi::eIDESection : uint8
{
	IDE_SECTION_UNKNOWN,
	IDE_SECTION_OBJS,
	IDE_SECTION_TOBJ,
	IDE_SECTION_HIER,
	IDE_SECTION_CARS,
	IDE_SECTION_PEDS,
	IDE_SECTION_PATH,
	IDE_SECTION_2DFX,
	IDE_SECTION_WEAP,
	IDE_SECTION_ANIM,
	IDE_SECTION_TXDP,
	IDE_SECTION_HAND,
	IDE_SECTION_TANM,
	IDE_SECTION_TREE,
	IDE_SECTION_VNOD,
	IDE_SECTION_LINK,
	IDE_SECTION_MLO,
	IDE_SECTION_AMAT,
	IDE_SECTION_LODM,
	IDE_SECTION_AGRPS
};