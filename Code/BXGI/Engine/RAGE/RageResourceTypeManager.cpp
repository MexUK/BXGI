#include "RageResourceTypeManager.h"
#include "Static/String.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

void											RageResourceTypeManager::init(void)
{
	initRageResourceTypes();
}
void											RageResourceTypeManager::uninit(void)
{
	removeAllEntries();
}

void											RageResourceTypeManager::initRageResourceTypes(void)
{
	RageResourceType *pRageResourceType = nullptr;

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_GENERIC);
	pRageResourceType->setIdentifier(0x01);
	pRageResourceType->setName("Generic");
	pRageResourceType->setFileExtension("UNKNOWN");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_TEXTUREARCHIVE);
	pRageResourceType->setIdentifier(0x08);
	pRageResourceType->setName("Texture Archive");
	pRageResourceType->setFileExtension("WTD");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_BOUNDS);
	pRageResourceType->setIdentifier(0x20);
	pRageResourceType->setName("Bounds");
	pRageResourceType->setFileExtension("UNKNOWN");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_XPFL);
	pRageResourceType->setIdentifier(0x24);
	pRageResourceType->setName("xpfl");
	pRageResourceType->setFileExtension("UNKNOWN");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_MODEL);
	pRageResourceType->setIdentifier(0x6e);
	pRageResourceType->setName("Model File");
	pRageResourceType->setFileExtension("WDD");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_MODEL);
	pRageResourceType->setIdentifier(0x6e);
	pRageResourceType->setName("Model File");
	pRageResourceType->setFileExtension("WDR");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_WINDOWSFRAG);
	pRageResourceType->setIdentifier(0x70);
	pRageResourceType->setName("Windows Frag");
	pRageResourceType->setFileExtension("WFT");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_PLACEMENT);
	pRageResourceType->setIdentifier(-1);
	pRageResourceType->setName("Placement File");
	pRageResourceType->setFileExtension("IPL");
	addEntry(pRageResourceType);

	pRageResourceType = new RageResourceType;
	pRageResourceType->setResourceTypeId(RAGE_RBS);
	pRageResourceType->setIdentifier(1297433423);
	pRageResourceType->setName("RBS");
	pRageResourceType->setFileExtension("RBS");
	addEntry(pRageResourceType);
}

RageResourceType*									RageResourceTypeManager::getResourceTypeByIdentifierOrFileExtension(uint32 uiResourceIdentifer, string strFileExtension)
{
	RageResourceType *pRageResourceType = nullptr;

	pRageResourceType = getResourceTypeByIdentifier(uiResourceIdentifer);
	if (pRageResourceType)
	{
		return pRageResourceType;
	}

	pRageResourceType = getResourceTypeByFileExtension(strFileExtension);
	if (pRageResourceType)
	{
		return pRageResourceType;
	}

	return nullptr;
}

RageResourceType*								RageResourceTypeManager::getResourceTypeByIdentifier(uint32 uiResourceIdentifer)
{
	for (auto pRageResource : getEntries())
	{
		if (pRageResource->getIdentifier() == uiResourceIdentifer)
		{
			return pRageResource;
		}
	}
	return nullptr;
}

RageResourceType*								RageResourceTypeManager::getResourceTypeByFileExtension(string strFileExtension)
{
	strFileExtension = String::toUpperCase(strFileExtension);
	for (auto pRageResource : getEntries())
	{
		if (String::toUpperCase(pRageResource->getFileExtension()) == strFileExtension)
		{
			return pRageResource;
		}
	}
	return nullptr;
}