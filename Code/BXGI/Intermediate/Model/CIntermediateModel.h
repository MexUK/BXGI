#pragma once

#include "nsbxgi.h"
#include "Pool/VectorPool.h"
#include "Data/CIntermediateGeometry.h"

class bxgi::CIntermediateModel : public bxcf::VectorPool<bxgi::CIntermediateGeometry*>
{
public:
	void								unload(void) {}
};