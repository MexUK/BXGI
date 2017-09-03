#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"
#include <string>
#include <vector>

class bxgi::CIDEFormat;

class bxgi::CIDEManager : public bxcf::FormatManager<bxgi::CIDEFormat>, public bxcf::Singleton<bxgi::CIDEManager>
{
public:
	CIDEManager(void);
	
	void								init(void);
	void								uninit(void);

	static std::vector<std::string>		getIDEEntryNamesWithoutExtension(std::vector<std::string> vecIDEPaths, bool bModelNames = true, bool bTXDNames = true);
};