#pragma once

#include "nsbxgi.h"
#include "Pool/VectorPool.h"
#include "CIntermediateModel.h"

class bxgi::CIntermediateModelFormat : public bxcf::VectorPool<bxgi::CIntermediateModel*>
{
public:
	void								unload(void);
};