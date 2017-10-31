#include "IDEManager.h"
#include "Format/IDE/IDEFormat.h"
#include "Format/IDE/EIDESection.h"
#include "Static/String.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"
#include "Game/EGame.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IDEManager::IDEManager(void)
{
}

// initialization
void							IDEManager::init(void)
{
}
void							IDEManager::uninit(void)
{
}

// entry fetching
void							IDEManager::getModelAndTextureSetNamesFromFiles(
	vector<string>& vecIDEFilePaths,
	set<string>& stModelNames,
	set<string>& stTextureSetNames,
	vector<EIDESection>& vecModelSections,
	vector<EIDESection>& vecTextureSections
)
{
	for (string& strIDEFilePath : vecIDEFilePaths)
	{
		IDEFormat ideFormat(strIDEFilePath);
		if (!ideFormat.unserialize())
		{
			continue;
		}

		for (string& strModelName : ideFormat.getModelNamesInSections(vecModelSections))
		{
			stModelNames.insert(String::toUpperCase(strModelName));
		}
		for (string& strTextureSetName : ideFormat.getTextureSetNamesInSections(vecTextureSections))
		{
			stTextureSetNames.insert(String::toUpperCase(strTextureSetName));
		}

		Events::trigger(TASK_PROGRESS);
	}
}

// entry fetching old
vector<string>					IDEManager::getIDEEntryNamesWithoutExtension(vector<string> vecIDEPaths, bool bModelNames, bool bTXDNames)
{
	vector<string> vecEntryNamesWithoutExtension;
	IDEFormat *pIDEFile = nullptr;
	for (string& strIDEPath : vecIDEPaths)
	{
		pIDEFile = IDEManager::get()->unserializeFile(strIDEPath);
		if (!pIDEFile)
		{
			delete pIDEFile;
			continue;
		}
		vector<string>
			vecModelNames,
			vecTextureNames;
		if (bModelNames)
		{
			vecModelNames = pIDEFile->getModelNames();
		}
		if (bTXDNames)
		{
			vecTextureNames = pIDEFile->getTXDNames();
		}
		StdVector::addToVector(vecEntryNamesWithoutExtension, vecModelNames);
		StdVector::addToVector(vecEntryNamesWithoutExtension, vecTextureNames);
		pIDEFile->unload();
		delete pIDEFile;
	}
	return StdVector::removeDuplicates(vecEntryNamesWithoutExtension);
}

// format version
string							IDEManager::getVersionText(uint32 uiFormatGames)
{
	vector<string> vecGameVersions;
	for (EGame uiGame : getFormatGames(uiFormatGames))
	{
		switch (uiGame)
		{
		case GTA_III:
			vecGameVersions.push_back("1");
			break;
		case GTA_VC:
			vecGameVersions.push_back("2");
			break;
		case GTA_SA:
			vecGameVersions.push_back("3");
			break;
		case GTA_IV:
			vecGameVersions.push_back("4");
			break;
		}
	}
	return String::join(vecGameVersions, ", ");
}

// format games
vector<EGame>					IDEManager::getFormatGames(uint32 uiFormatGames)
{
	vector<EGame> vecGames;
	if (uiFormatGames & GAME_FLAG_GTA_III)
	{
		vecGames.push_back(GTA_III);
	}
	if (uiFormatGames & GAME_FLAG_GTA_VC)
	{
		vecGames.push_back(GTA_VC);
	}
	if (uiFormatGames & GAME_FLAG_GTA_SA)
	{
		vecGames.push_back(GTA_SA);
	}
	if (uiFormatGames & GAME_FLAG_GTA_IV)
	{
		vecGames.push_back(GTA_IV);
	}
	return vecGames;
}

string							IDEManager::getFormatGamesAsString(uint32 uiFormatGames)
{
	vector<string> vecGameNames;
	for (EGame uiGame : getFormatGames(uiFormatGames))
	{
		switch (uiGame)
		{
		case GTA_III:
			vecGameNames.push_back("GTA III");
			break;
		case GTA_VC:
			vecGameNames.push_back("GTA VC");
			break;
		case GTA_SA:
			vecGameNames.push_back("GTA SA");
			break;
		case GTA_IV:
			vecGameNames.push_back("GTA IV");
			break;
		}
	}
	return String::join(vecGameNames, ", ");
}