#include "DATLoaderManager.h"
#include "Static/String2.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

DATLoaderManager::DATLoaderManager(void)
{
}

void				DATLoaderManager::init(void)
{
}
void				DATLoaderManager::uninit(void)
{
}

EDATLoaderEntryType DATLoaderManager::getDATEntryTypeFromString(string strType)
{
	strType = String2::toUpperCase(strType);
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