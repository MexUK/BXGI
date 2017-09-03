#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"
#include "EDATLoaderEntryType.h"
#include "Game/EPlatformedGame.h"
#include <string>

class bxgi::CDATLoaderFormat;

class bxgi::CDATLoaderManager : public bxcf::FormatManager<bxgi::CDATLoaderFormat>, public bxcf::Singleton<bxgi::CDATLoaderManager>
{
public:
	CDATLoaderManager(void);

	void						init(void);
	void						uninit(void);

	static EDATLoaderEntryType	getDATEntryTypeFromString(std::string strType);
	static std::string			getDefaultGameDATSubPath(bxgi::EPlatformedGame EPlatformedGameValue);
};