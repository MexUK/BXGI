#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/Format.h"
#include "Pool/VectorPool.h"
#include "CDATScriptPathEntry.h"

class bxgi::CDATScriptPathFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::CDATScriptPathEntry*>
{
public:
	CDATScriptPathFormat(void);
	
	void					unserialize(void);
	void					serialize(void); // todo
};