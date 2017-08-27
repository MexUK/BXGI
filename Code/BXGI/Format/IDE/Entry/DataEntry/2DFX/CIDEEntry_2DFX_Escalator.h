#pragma once

#include "nsbxgi.h"
#include "Format/IDE/Entry/DataEntry/CIDEEntry_2DFX.h"

class bxgi::CIDEEntry_2DFX_Escalator : public bxgi::CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_Escalator(bxgi::CIDEFormat *pIDEFormat);

	void						unserialize(void);
	void						serialize(void);
};