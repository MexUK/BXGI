#include "CDATLoaderManager.h"
#include "Static/CString2.h"
#include "Static/CStdVector.h"
#include "Static/CDebug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CDATLoaderManager::CDATLoaderManager(void)
{
}

void				CDATLoaderManager::init(void)
{
}
void				CDATLoaderManager::uninit(void)
{
}

EDATLoaderEntryType CDATLoaderManager::getDATEntryTypeFromString(string strType)
{
	strType = CString2::toUpperCase(strType);
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

string			CDATLoaderManager::getDefaultGameDATSubPath(EPlatformedGame EPlatformedGameValue)
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