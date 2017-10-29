#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"
#include <string>
#include <vector>
#include <set>

class bxgi::IDEFormat;

class bxgi::IDEManager : public bxcf::FormatManager<bxgi::IDEFormat>, public bxcf::Singleton<bxgi::IDEManager>
{
public:
	IDEManager(void);
	
	void								init(void);
	void								uninit(void);

	static void							getModelAndTextureSetNamesFromFiles(
		std::vector<std::string>& vecIDEFilePaths,
		std::set<std::string>& stModelNames,
		std::set<std::string>& stTextureSetNames,
		std::vector<EIDESection>& vecModelSections,
		std::vector<EIDESection>& vecTextureSections
	);

	static std::vector<std::string>		getIDEEntryNamesWithoutExtension(std::vector<std::string> vecIDEPaths, bool bModelNames = true, bool bTXDNames = true);

	static std::string					getVersionText(uint32 uiFormatGames);

	static std::vector<EGame>			getFormatGames(uint32 uiFormatGames);
	static std::string					getFormatGamesAsString(uint32 uiFormatGames);
};