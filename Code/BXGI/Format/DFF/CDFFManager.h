#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"

class bxgi::CDFFFormat;

class bxgi::CDFFManager : public bxcf::FormatManager<bxgi::CDFFFormat>, public bxcf::Singleton<bxgi::CDFFManager>
{
public:
	CDFFManager(void);

	void						init(void);
	void						uninit(void);
};