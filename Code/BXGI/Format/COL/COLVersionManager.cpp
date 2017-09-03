#include "COLVersionManager.h"
#include "COLVersion.h"
#include "Static/String.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

// initialization
void											COLVersionManager::init(void)
{
	initCOLVersions();
}

void											COLVersionManager::uninit(void)
{
	uninitCOLVersions();
}

// version initialization
void											COLVersionManager::initCOLVersions(void)
{
	addVersion(COL_UNKNOWN, "COL Unknown");
	addVersion(COL_1, "COL 1", { GTA_III });
	addVersion(COL_2, "COL 2", { GTA_VC });
	addVersion(COL_3, "COL 3", { GTA_SA });
	addVersion(COL_4, "COL 4");
}

void											COLVersionManager::uninitCOLVersions(void)
{
	removeAllEntries();
}

// add/remove version
COLVersion*									COLVersionManager::addVersion(ECOLVersion uiCOLVersion, string strVersionText, vector<EGame> vecGames)
{
	COLVersion *pCOLVersion = new COLVersion;
	pCOLVersion->setVersionId(uiCOLVersion);
	pCOLVersion->setText(strVersionText);
	pCOLVersion->setGames(vecGames);
	addEntry(pCOLVersion);
	return pCOLVersion;
}

// version text
string											COLVersionManager::getVersionText(uint32 uiRawVersion)
{
	if (m_umapRawVersionTexts.count(uiRawVersion))
	{
		return m_umapRawVersionTexts[uiRawVersion];
	}
	else
	{
		string strVersionText = "COL " + String::toString(uiRawVersion);
		m_umapRawVersionTexts[uiRawVersion] = strVersionText;
		return strVersionText;
	}
}

// entry fetching
COLVersion*									COLVersionManager::getEntryByVersionId(ECOLVersion ECOLVersionValue)
{
	for (COLVersion *pCOLVersion : getEntries())
	{
		if (pCOLVersion->getVersionId() == ECOLVersionValue)
		{
			return pCOLVersion;
		}
	}
	return nullptr;
}