#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/IPL/Entry/CIPLEntry_Data.h"

class bxgi::CIPLEntry_LINK : public bxgi::CIPLEntry_Data
{
public:
	CIPLEntry_LINK(bxgi::CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_LINK)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};