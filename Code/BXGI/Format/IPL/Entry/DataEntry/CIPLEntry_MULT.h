#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/IPL/Entry/CIPLEntry_Data.h"

class bxgi::CIPLEntry_MULT : public bxgi::CIPLEntry_Data
{
public:
	CIPLEntry_MULT(bxgi::CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_MULT)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};