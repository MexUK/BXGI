#pragma once

#include "nsbxgi.h"
#include "Pool/VectorPool.h"
#include "Format/RW/Entries/C2dEffect.h"

class bxgi::C2dEffectsPool : public bxcf::VectorPool<bxgi::C2dEffect*>
{
};