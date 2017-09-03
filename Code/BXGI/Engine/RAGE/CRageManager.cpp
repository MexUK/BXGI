#include "CRageManager.h"
#include "Static/String2.h"
#include "CRageResourceTypeManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CRageManager::CRageManager(void)
{
	m_pResourceTypeManager = new CRageResourceTypeManager;
}

CRageManager::~CRageManager(void)
{
	delete m_pResourceTypeManager;
}

void											CRageManager::init(void)
{
	getResourceTypeManager()->init();
}
void											CRageManager::uninit(void)
{
	getResourceTypeManager()->uninit();
}

uint32											CRageManager::getPackedOffset(uint32 uiOffset)
{
	return uiOffset | (5 << 28);
}

uint32											CRageManager::getPackedDataOffset(uint32 uiOffset)
{
	return uiOffset | (6 << 28);
}

// file extension
bool											CRageManager::isRageFileExtension(string& strFileExtension)
{
	return isRageModelExtension(strFileExtension)
		|| isRageTextureExtension(strFileExtension)
		|| isRageCollisionExtension(strFileExtension);
}

bool											CRageManager::isRageModelExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "WDR";
}

bool											CRageManager::isRageTextureExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "WTD";
}

bool											CRageManager::isRageCollisionExtension(string& strFileExtension)
{
	string strExtensionUpper = String2::toUpperCase(strFileExtension);
	return strExtensionUpper == "WBN";
}