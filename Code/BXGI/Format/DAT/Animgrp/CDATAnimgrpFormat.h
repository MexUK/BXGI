#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/Format.h"
#include "CDATAnimgrpSection.h"

class bxgi::CDATAnimgrpFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::CDATAnimgrpSection*>
{
public:
	CDATAnimgrpFormat(void);
	
	void					unserialize(void);
	void					serialize(void);
};