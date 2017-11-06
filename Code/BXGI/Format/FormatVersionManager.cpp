#include "FormatVersionManager.h"
#include "Format/FormatVersion.h"
#include "Format/COL/COLManager.h"
#include "Format/COL/COLVersionManager.h"
#include "Format/COL/COLVersion.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersionManager.h"
#include "Engine/RW/RWVersion.h"

using namespace std;
using namespace bxcf;
using namespace bxcf::fileType;
using namespace bxgi;

// initialization
void					FormatVersionManager::init(void)
{
	initFormatVersions();
}

void					FormatVersionManager::initFormatVersions(void)
{
	addFormatVersion(ANIMATION, 1, string("IFP 1"));
	addFormatVersion(ANIMATION, 2, string("IFP 2"));
	addFormatVersion(ANIMATION, 3, string("IFP 3"));

	for (COLVersion *pCOLVersion : COLManager::get()->getVersionManager()->getEntries())
	{
		addFormatVersion(COLLISION, pCOLVersion->getVersionId(), pCOLVersion->getText());
	}

	for (RWVersion *pRWVersion : RWManager::get()->getVersionManager()->getEntries())
	{
		addFormatVersion(TEXTURE, pRWVersion->getRawVersion(), pRWVersion->getVersionText());
	}
}

// add entry
FormatVersion*			FormatVersionManager::addFormatVersion(EFileType uiFileType, uint32 uiRawVersion, string& strVersionText)
{
	FormatVersion *pFormatVersion = new FormatVersion;
	pFormatVersion->m_uiFileType = uiFileType;
	pFormatVersion->m_uiRawVersion = uiRawVersion;
	pFormatVersion->m_strVersionText = strVersionText;
	addEntry(pFormatVersion);
	return pFormatVersion;
}

// fetch versions
vector<string>			FormatVersionManager::getEntriesVersionsText(void)
{
	vector<string> vecEntriesVersionsText;
	for (FormatVersion *pFormatVersion : getEntries())
	{
		vecEntriesVersionsText.push_back(pFormatVersion->m_strVersionText);
	}
	return vecEntriesVersionsText;
}

// fetch entry
FormatVersion*			FormatVersionManager::getEntryByVersionText(string& strVersionText)
{
	for (FormatVersion *pFormatVersion : getEntries())
	{
		if (strVersionText == pFormatVersion->m_strVersionText)
		{
			return pFormatVersion;
		}
	}
	return nullptr;
}