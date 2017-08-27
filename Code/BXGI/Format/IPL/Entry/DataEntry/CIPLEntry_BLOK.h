#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/IPL/Entry/CIPLEntry_Data.h"

class bxgi::CIPLEntry_BLOK : public bxgi::CIPLEntry_Data
{
public:
	CIPLEntry_BLOK(bxgi::CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_BLOK)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};