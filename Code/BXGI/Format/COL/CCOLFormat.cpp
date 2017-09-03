#include "CCOLFormat.h"
#include "Static/String2.h"
#include "Static/File.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "CCOLManager.h"
#include "Static/Debug.h"
#include <algorithm>

using namespace std;
using namespace bxcf;
using namespace bxgi;

void				CCOLFormat::unload(void)
{
	removeAllEntries();
}

void				CCOLFormat::unserialize(void)
{
	while (!DataReader::get()->isEOF())
	{
		CCOLEntry *pCOLEntry = new CCOLEntry(this);
		m_vecEntries.push_back(pCOLEntry);
		pCOLEntry->unserialize();
	}
}

void				CCOLFormat::serialize(void)
{
	for (CCOLEntry *pCOLEntry : getEntries())
	{
		pCOLEntry->serialize();
	}
}

void				CCOLFormat::setCOLVersion(ECOLVersion uiCOLVersion)
{
	for (auto pCOLEntry : getEntries())
	{
		pCOLEntry->setCOLVersion(uiCOLVersion);
	}
}

vector<string>		CCOLFormat::getModelNames(void)
{
	vector<string> vecModelNames;
	for (auto pCOLEntry : getEntries())
	{
		vecModelNames.push_back(pCOLEntry->getModelName());
	}
	return vecModelNames;
}

CCOLEntry*			CCOLFormat::getEntryByName(string strName)
{
	strName = String2::toUpperCase(strName);
	for (auto pCOLEntry : getEntries())
	{
		if (strName == String2::toUpperCase(pCOLEntry->getModelName()))
		{
			return pCOLEntry;
		}
	}
	return nullptr;
}

vector<CCOLEntry*>	CCOLFormat::getEntriesByModelName(string strModelName)
{
	vector<CCOLEntry*> vecCOLEntries;
	strModelName = String2::toUpperCase(strModelName);
	for (auto pCOLEntry : getEntries())
	{
		if (String2::toUpperCase(pCOLEntry->getModelName()) == strModelName)
		{
			vecCOLEntries.push_back(pCOLEntry);
		}
	}
	return vecCOLEntries;
}

void				CCOLFormat::sort(void)
{
	std::sort(getEntries().begin(), getEntries().end(), sortFunction);
}

bool				CCOLFormat::sortFunction(CCOLEntry *pCOLEntry1, CCOLEntry *pCOLEntry2)
{
	return pCOLEntry1->getModelName() < pCOLEntry2->getModelName();
}