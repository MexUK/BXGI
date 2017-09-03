#include "COLManager.h"
#include "COLFormat.h"
#include "Stream/DataReader.h"
#include "Static/String2.h"
#include "COLVersionManager.h"
#include "Static/File.h"
#include "Localization/LocalizationManager.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

COLManager::COLManager(void)
{
	m_pVersionManager = new COLVersionManager;
}
COLManager::~COLManager(void)
{
	delete m_pVersionManager;
}

void			COLManager::init(void)
{
	m_pVersionManager->init();
	initGameMaterials();
}
void			COLManager::uninit(void)
{
	m_pVersionManager->uninit();
}

void			COLManager::initGameMaterials(void)
{
	// todo
	/*
	umapGameMaterialIds[GTA_III] = {
		{ "grass", 2 },
		{ "dirt", 3 },
		{ "mud", 4 },
		{ "pavement", 5 },
		{ "metal", 6 },
		{ "sand", 18 },
		{ "hedge", 25 },
		{ "stone", 26 }
	};
	umapGameMaterialIds[GTA_VC] = {
		{ "grass", 2 },
		{ "dirt", 3 },
		{ "mud", 4 },
		{ "pavement", 5 },
		{ "metal", 6 },
		{ "glass", 7 },
		{ "metal door", 9 },
		{ "metal fence", 16 },
		{ "sand", 18 },
		{ "water", 19 },
		{ "wood", 20 },
		{ "wood box", 21 },
		{ "wood plank", 22 },
		{ "hedge", 25 },
		{ "stone", 26 },
		{ "bollard", 31 },
		{ "gate", 32 }
	};
	*/
}

ECOLVersion			COLManager::getCOLVersionFromFourCC(string strFourCC)
{
	ECOLVersion ECOLVersionValue;
	if (strFourCC == "COLL")
	{
		ECOLVersionValue = COL_1;
	}
	else if (strFourCC == "COL2")
	{
		ECOLVersionValue = COL_2;
	}
	else if (strFourCC == "COL3")
	{
		ECOLVersionValue = COL_3;
	}
	else if (strFourCC == "COL4")
	{
		ECOLVersionValue = COL_4;
	}
	else
	{
		ECOLVersionValue = COL_UNKNOWN;
	}
	return ECOLVersionValue;
}

string			COLManager::getCOLVersionText(ECOLVersion ECOLVersionValue)
{
	COLVersion *pCOLVersion = get()->getVersionManager()->getEntryByVersionId(ECOLVersionValue);
	return pCOLVersion == nullptr ? "Unknown COL Version" : pCOLVersion->getText();
}

string			COLManager::getFourCCFromCOLVersion(ECOLVersion ECOLVersionValue)
{
	switch (ECOLVersionValue)
	{
	case COL_1: return "COLL";
	case COL_2: return "COL2";
	case COL_3: return "COL3";
	case COL_4: return "COL4";

	case COL_UNKNOWN:
	default:
		return "COLU";
	}
}

uint32		COLManager::getEntryHeaderSizeForPacking(ECOLVersion ECOLVersionValue)
{
	uint32 uiHeaderPart1Size = 4 + 4 + 22 + 2 + 40;
	uint32 uiHeaderPart2Size_COLVersion2 = (3 * 2) + (2 * 1) + (7 * 4);
	uint32 uiFinalValue = 0;
	switch (ECOLVersionValue)
	{
	case COL_1:
		uiFinalValue = uiHeaderPart1Size;
		break;
	case COL_2:
		uiFinalValue = uiHeaderPart1Size + uiHeaderPart2Size_COLVersion2; // 108
		break;
	case COL_3:
		uiFinalValue = uiHeaderPart1Size + uiHeaderPart2Size_COLVersion2 + 12;
		break;
	case COL_4:
		uiFinalValue = uiHeaderPart1Size + uiHeaderPart2Size_COLVersion2 + 16;
		break;
	}
	uiFinalValue -= 8;
	return uiFinalValue;
}

// file extension
bool											COLManager::isCollisionExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "COL";
}