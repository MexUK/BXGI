#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/IPL/Entry/CIPLEntry_Data.h"

class bxgi::CIPLEntry_LODM : public bxgi::CIPLEntry_Data
{
public:
	CIPLEntry_LODM(bxgi::CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_LODM)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};