#include "DATLoaderManager.h"
#include "Format/DAT/Loader/DATLoaderFormat.h"
#include "Static/String.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

DATLoaderManager::DATLoaderManager(void)
{
}

// initialization
void				DATLoaderManager::init(void)
{
}
void				DATLoaderManager::uninit(void)
{
}

// IDE paths
vector<string>		DATLoaderManager::getRelativeIDEFilePaths(vector<string>& vecDATFilePaths)
{
	vector<string> vecIDEFilePaths;
	for (string& strDATFilePath : vecDATFilePaths)
	{
		DATLoaderFormat datFile(strDATFilePath);
		if (datFile.unserialize())
		{
			StdVector::addToVector(vecIDEFilePaths, datFile.getRelativeIDEPaths());
		}
		datFile.unload();
	}
	return vecIDEFilePaths;
}

vector<string>		DATLoaderManager::getIDEFilePaths(string& strFolderPath, vector<string>& vecDATFilePaths)
{
	vector<string> vecIDEFilePaths = getRelativeIDEFilePaths(vecDATFilePaths);
	for (string& strIDEFilePath : vecIDEFilePaths)
	{
		strIDEFilePath = strFolderPath + strIDEFilePath;
	}
	return vecIDEFilePaths;
}

// DAT entry type
EDATLoaderEntryType DATLoaderManager::getDATEntryTypeFromString(string strType)
{
	strType = String::toUpperCase(strType);
	if (strType == "IMG")
		return DAT_LOADER_IMG;
	if (strType == "CDIMAGE")
		return DAT_LOADER_CDIMAGE;
	if (strType == "IMGLIST")
		return DAT_LOADER_IMGLIST;
	if (strType == "WATER")
		return DAT_LOADER_WATER;
	if (strType == "IDE")
		return DAT_LOADER_IDE;
	if (strType == "HIERFILE")
		return DAT_LOADER_HIERFILE;
	if (strType == "COLFILE")
		return DAT_LOADER_COLFILE;
	if (strType == "IPL")
		return DAT_LOADER_IPL;
	if (strType == "MAPZONE")
		return DAT_LOADER_MAPZONE;
	if (strType == "TEXDICTION")
		return DAT_LOADER_TEXDICTION;
	if (strType == "MODELFILE")
		return DAT_LOADER_MODELFILE;
	if (strType == "SPLASH")
		return DAT_LOADER_SPLASH;
	if (strType == "EXIT")
		return DAT_LOADER_EXIT;
	return DAT_LOADER_UNKNOWN;
}

// game default DAT path
string			DATLoaderManager::getDefaultGameDATSubPath(EPlatformedGame EPlatformedGameValue)
{
	switch (EPlatformedGameValue)
	{
	case PC_GTA_III:	return "data/gta3.dat";
	case PC_GTA_VC:		return "data/gta_vc.dat";
	case PC_GTA_SA:		return "data/gta.dat";
	case PC_GTA_IV:		return "common/data/gta.dat";
	case PC_SOL:		return "sol/gta_sol.dat";
	}
	return "";
}