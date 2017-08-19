#ifndef CIDEEntry_2DFX_WorldPoint_H
#define CIDEEntry_2DFX_WorldPoint_H

#include "bxgi.h"
#include "Format/IDE/Entry/DataEntry/CIDEEntry_2DFX.h"
#include <string>

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

#endif