#include "CCOLVersionManager.h"
#include "CCOLVersion.h"
#include "Static/CString2.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

// initialization
void											CCOLVersionManager::init(void)
{
	initCOLVersions();
}

void											CCOLVersionManager::uninit(void)
{
	uninitCOLVersions();
}

// version initialization
void											CCOLVersionManager::initCOLVersions(void)
{
	addVersion(COL_UNKNOWN, "COL Unknown");
	addVersion(COL_1, "COL 1", { GTA_III });
	addVersion(COL_2, "COL 2", { GTA_VC });
	addVersion(COL_3, "COL 3", { GTA_SA });
	addVersion(COL_4, "COL 4");
}

void											CCOLVersionManager::uninitCOLVersions(void)
{
	removeAllEntries();
}

// add/remove version
CCOLVersion*									CCOLVersionManager::addVersion(ECOLVersion uiCOLVersion, string strVersionText, vector<EGame> vecGames)
{
	CCOLVersion *pCOLVersion = new CCOLVersion;
	pCOLVersion->setVersionId(uiCOLVersion);
	pCOLVersion->setText(strVersionText);
	pCOLVersion->setGames(vecGames);
	addEntry(pCOLVersion);
	return pCOLVersion;
}

// version text
string											CCOLVersionManager::getVersionText(uint32 uiRawVersion)
{
	if (m_umapRawVersionTexts.count(uiRawVersion))
	{
		return m_umapRawVersionTexts[uiRawVersion];
	}
	else
	{
		string strVersionText = "COL " + CString2::toString(uiRawVersion);
		m_umapRawVersionTexts[uiRawVersion] = strVersionText;
		return strVersionText;
	}
}

// entry fetching
CCOLVersion*									CCOLVersionManager::getEntryByVersionId(ECOLVersion ECOLVersionValue)
{
	for (CCOLVersion *pCOLVersion : getEntries())
	{
		if (pCOLVersion->getVersionId() == ECOLVersionValue)
		{
			return pCOLVersion;
		}
	}
	return nullptr;
}