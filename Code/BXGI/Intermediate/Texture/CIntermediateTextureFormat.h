#pragma once

#include "nsbxgi.h"
#include "Pool/VectorPool.h"
#include "CIntermediateTexture.h"

class bxgi::CIntermediateTextureFormat : public bxcf::VectorPool<bxgi::CIntermediateTexture*>
{
public:
	void						unload(void);
};