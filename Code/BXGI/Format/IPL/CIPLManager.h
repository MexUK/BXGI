#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"

class bxgi::CIPLFormat;

class bxgi::CIPLManager : public bxcf::FormatManager<bxgi::CIPLFormat>, public bxcf::Singleton<bxgi::CIPLManager>
{
public:
	CIPLManager(void);
	
	void					init(void);
	void					uninit(void);
};