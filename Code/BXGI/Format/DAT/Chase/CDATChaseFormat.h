#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/Format.h"
#include "Pool/VectorPool.h"
#include "CDATChaseEntry.h"
#include <vector>

class bxgi::CDATChaseFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::CDATChaseEntry*>
{
public:
	CDATChaseFormat(void);
	
	void					unserialize(void);
	void					serialize(void);
};