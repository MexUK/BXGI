#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"
#include "EDATLoaderEntryType.h"
#include "Game/EPlatformedGame.h"
#include <string>

class bxgi::DATLoaderFormat;

class bxgi::DATLoaderManager : public bxcf::FormatManager<bxgi::DATLoaderFormat>, public bxcf::Singleton<bxgi::DATLoaderManager>
{
public:
	DATLoaderManager(void);

	void						init(void);
	void						uninit(void);

	static std::vector<std::string>		getRelativeIDEFilePaths(std::vector<std::string>& vecDATFilePaths);
	static std::vector<std::string>		getIDEFilePaths(std::string& strFolderPath, std::vector<std::string>& vecDATFilePaths);

	static EDATLoaderEntryType			getDATEntryTypeFromString(std::string strType);
	static std::string					getDefaultGameDATSubPath(bxgi::EPlatformedGame EPlatformedGameValue);
};