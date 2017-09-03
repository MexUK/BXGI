#include "COLFormat.h"
#include "Static/String2.h"
#include "Static/File.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "COLManager.h"
#include "Static/Debug.h"
#include <algorithm>

using namespace std;
using namespace bxcf;
using namespace bxgi;

void				COLFormat::unload(void)
{
	removeAllEntries();
}

void				COLFormat::unserialize(void)
{
	while (!DataReader::get()->isEOF())
	{
		COLEntry *pCOLEntry = new COLEntry(this);
		m_vecEntries.push_back(pCOLEntry);
		pCOLEntry->unserialize();
	}
}

void				COLFormat::serialize(void)
{
	for (COLEntry *pCOLEntry : getEntries())
	{
		pCOLEntry->serialize();
	}
}

void				COLFormat::setCOLVersion(ECOLVersion uiCOLVersion)
{
	for (auto pCOLEntry : getEntries())
	{
		pCOLEntry->setCOLVersion(uiCOLVersion);
	}
}

vector<string>		COLFormat::getModelNames(void)
{
	vector<string> vecModelNames;
	for (auto pCOLEntry : getEntries())
	{
		vecModelNames.push_back(pCOLEntry->getModelName());
	}
	return vecModelNames;
}

COLEntry*			COLFormat::getEntryByName(string strName)
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

vector<COLEntry*>	COLFormat::getEntriesByModelName(string strModelName)
{
	vector<COLEntry*> vecCOLEntries;
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

void				COLFormat::sort(void)
{
	std::sort(getEntries().begin(), getEntries().end(), sortFunction);
}

bool				COLFormat::sortFunction(COLEntry *pCOLEntry1, COLEntry *pCOLEntry2)
{
	return pCOLEntry1->getModelName() < pCOLEntry2->getModelName();
}