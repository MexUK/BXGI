#pragma once

#include "nsbxgi.h"
#include "Format/IDE/Entry/DataEntry/CIDEEntry_2DFX.h"

class bxgi::CIDEEntry_2DFX_WorldPoint : public bxgi::CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_WorldPoint(bxgi::CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, bxgi::_2DFX_WORLD_POINT)
	{
	}

	void						unserialize(void);
	void						serialize(void);
};