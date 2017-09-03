#include "RWManager.h"
#include "RWVersionManager.h"
#include "Format/RW/RWSection.h"
#include "Localization/LocalizationManager.h"
#include "Static/String.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWManager::RWManager(void) :
	m_uiSerializationRWVersion(0)
{
	m_pVersionManager = new RWVersionManager;
}
RWManager::~RWManager(void)
{
	delete m_pVersionManager;
}

// initialization
void											RWManager::init(void)
{
	m_pVersionManager->init();
	RWSection::initStatic();
}
void											RWManager::uninit(void)
{
	m_pVersionManager->uninit();
}

// file extension
bool											RWManager::isRWFileExtension(string& strFileExtension)
{
	return isRWModelExtension(strFileExtension) || isRWTextureExtension(strFileExtension);
}

bool											RWManager::isRWModelExtension(string& strFileExtension)
{
	string strExtensionUpper = String::toUpperCase(strFileExtension);
	return strExtensionUpper == "DFF"
		|| strExtensionUpper == "BSP";
}

bool											RWManager::isRWTextureExtension(string& strFileExtension)
{
	string strExtensionUpper = String::toUpperCase(strFileExtension);
	return strExtensionUpper == "TXD";
}