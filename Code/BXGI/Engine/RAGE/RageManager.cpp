#include "RageManager.h"
#include "Static/String2.h"
#include "RageResourceTypeManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RageManager::RageManager(void)
{
	m_pResourceTypeManager = new RageResourceTypeManager;
}

RageManager::~RageManager(void)
{
	delete m_pResourceTypeManager;
}

void											RageManager::init(void)
{
	getResourceTypeManager()->init();
}
void											RageManager::uninit(void)
{
	getResourceTypeManager()->uninit();
}

uint32											RageManager::getPackedOffset(uint32 uiOffset)
{
	return uiOffset | (5 << 28);
}

uint32											RageManager::getPackedDataOffset(uint32 uiOffset)
{
	return uiOffset | (6 << 28);
}

// file extension
bool											RageManager::isRageFileExtension(string& strFileExtension)
{
	return isRageModelExtension(strFileExtension)
		|| isRageTextureExtension(strFileExtension)
		|| isRageCollisionExtension(strFileExtension);
}

bool											RageManager::isRageModelExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "WDR";
}

bool											RageManager::isRageTextureExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "WTD";
}

bool											RageManager::isRageCollisionExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "WBN";
}