#pragma once

#include "bxgi.h"
#include "Pool/CVectorPool.h"
#include "CIntermediateModel.h"

class bxgi::CIntermediateModelFormat : public bxcf::CVectorPool<bxgi::CIntermediateModel*>
{
public:
	void								unload(void);
};