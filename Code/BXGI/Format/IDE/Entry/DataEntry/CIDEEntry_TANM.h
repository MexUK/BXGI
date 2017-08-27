#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/IDE/Entry/CIDEEntry_Data.h"

class bxgi::CIDEEntry_TANM : public bxgi::CIDEEntry_Data
{
public:
	CIDEEntry_TANM(bxgi::CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_TANM)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};