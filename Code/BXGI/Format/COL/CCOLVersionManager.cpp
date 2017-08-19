#include "CCOLVersionManager.h"
#include "CCOLVersion.h"
#include "Static/CString2.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

void											CCOLVersionManager::init(void)
{
	initCOLVersions();
}

void											CCOLVersionManager::uninit(void)
{
	uninitCOLVersions();
}

void											CCOLVersionManager::initCOLVersions(void)
{
	CCOLVersion *pCOLVersion = nullptr;

	pCOLVersion = new CCOLVersion;
	pCOLVersion->setVersionId(COL_1);
	pCOLVersion->setText("COL 1 (GTA III)");
	pCOLVersion->setLocalizationKey("COL_1");
	addEntry(pCOLVersion);

	pCOLVersion = new CCOLVersion;
	pCOLVersion->setVersionId(COL_2);
	pCOLVersion->setText("COL 2 (GTA VC)");
	pCOLVersion->setLocalizationKey("COL_2");
	addEntry(pCOLVersion);

	pCOLVersion = new CCOLVersion;
	pCOLVersion->setVersionId(COL_3);
	pCOLVersion->setText("COL 3 (GTA SA)");
	pCOLVersion->setLocalizationKey("COL_3");
	addEntry(pCOLVersion);

	pCOLVersion = new CCOLVersion;
	pCOLVersion->setVersionId(COL_4);
	pCOLVersion->setText("COL 4");
	pCOLVersion->setLocalizationKey("COL_4");
	addEntry(pCOLVersion);
}

void											CCOLVersionManager::uninitCOLVersions(void)
{
	removeAllEntries();
}

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

CCOLVersion*									CCOLVersionManager::getEntryByVersionId(eCOLVersion eCOLVersionValue)
{
	for (CCOLVersion *pCOLVersion : getEntries())
	{
		if (pCOLVersion->getVersionId() == eCOLVersionValue)
		{
			return pCOLVersion;
		}
	}
	return nullptr;
}