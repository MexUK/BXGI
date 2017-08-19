#include "CRWManager.h"
#include "CRWVersionManager.h"
#include "CRWSection.h"
#include "Localization/CLocalizationManager.h"
#include "Static/CString2.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CRWManager::CRWManager(void) :
	m_uiRWVersionCCForSerialization(0)
{
	m_pVersionManager = new CRWVersionManager;
}
CRWManager::~CRWManager(void)
{
	delete m_pVersionManager;
}


void											CRWManager::init(void)
{
	m_pVersionManager->init();
	CRWSection::initStatic();
}
void											CRWManager::uninit(void)
{
	m_pVersionManager->uninit();
}

string											CRWManager::getRWVersionText(eRWVersion eRWVersionValue)
{
	CRWVersion *pRWVersion = get()->getVersionManager()->getEntryByVersionId(eRWVersionValue);
	return pRWVersion == nullptr ? "Unknown RW Version" : pRWVersion->getText();
}

string											CRWManager::getGameName(ePlatformedGame ePlatformedGame)
{
	switch (ePlatformedGame)
	{
	case PLATFORMED_GAME_PC_GTA_III:		return "[PC] GTA III";
	case PLATFORMED_GAME_PC_GTA_VC:			return "[PC] GTA VC";
	case PLATFORMED_GAME_PC_GTA_SA:			return "[PC] GTA SA";
	case PLATFORMED_GAME_XBOX_GTA:			return "[XBOX] GTA";
	case PLATFORMED_GAME_ANDROID_GTA:		return "[Android] GTA";
	case PLATFORMED_GAME_PC_MANHUNT:		return "[PC] Manhunt";
	case PLATFORMED_GAME_UNKNOWN:
	default:
		return CLocalizationManager::get()->getTranslatedText("UnknownGame");
	}
}

// file extension
bool											CRWManager::isRWFileExtension(string& strFileExtension)
{
	return isRWModelExtension(strFileExtension) || isRWTextureExtension(strFileExtension);
}

bool											CRWManager::isRWModelExtension(string& strFileExtension)
{
	string strExtensionUpper = CString2::toUpperCase(strFileExtension);
	return strExtensionUpper == "DFF"
		|| strExtensionUpper == "BSP";
}

bool											CRWManager::isRWTextureExtension(string& strFileExtension)
{
	string strExtensionUpper = CString2::toUpperCase(strFileExtension);
	return strExtensionUpper == "TXD";
}