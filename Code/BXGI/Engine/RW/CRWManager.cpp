#include "CRWManager.h"
#include "CRWVersionManager.h"
#include "Format/RW/CRWSection.h"
#include "Localization/CLocalizationManager.h"
#include "Static/CString2.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CRWManager::CRWManager(void) :
	m_uiSerializationRWVersion(0)
{
	m_pVersionManager = new CRWVersionManager;
}
CRWManager::~CRWManager(void)
{
	delete m_pVersionManager;
}

// initialization
void											CRWManager::init(void)
{
	m_pVersionManager->init();
	CRWSection::initStatic();
}
void											CRWManager::uninit(void)
{
	m_pVersionManager->uninit();
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