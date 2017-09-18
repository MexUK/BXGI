#include "COLFormat.h"
#include "Static/String.h"
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

// serialization
void				COLFormat::_unserialize(void)
{
	while (!m_reader.isEOF())
	{
		COLEntry *pCOLEntry = new COLEntry(this);
		m_vecEntries.push_back(pCOLEntry);
		if (!pCOLEntry->unserialize())
		{
			// EOF (multiple \0's)
			m_vecEntries.erase(m_vecEntries.end() - 1);
			delete pCOLEntry;
			break;
		}
	}
}

void				COLFormat::_serialize(void)
{
	for (COLEntry *pCOLEntry : getEntries())
	{
		pCOLEntry->serialize();
	}
}

// COL version
void				COLFormat::setCOLVersion(ECOLVersion uiCOLVersion)
{
	for (auto pCOLEntry : getEntries())
	{
		pCOLEntry->setCOLVersion(uiCOLVersion);
	}
}

// model names
vector<string>		COLFormat::getModelNames(void)
{
	vector<string> vecModelNames;
	for (auto pCOLEntry : getEntries())
	{
		vecModelNames.push_back(pCOLEntry->getModelName());
	}
	return vecModelNames;
}

// fetch entry
COLEntry*			COLFormat::getEntryByName(string strName)
{
	strName = String::toUpperCase(strName);
	for (auto pCOLEntry : getEntries())
	{
		if (strName == String::toUpperCase(pCOLEntry->getModelName()))
		{
			return pCOLEntry;
		}
	}
	return nullptr;
}

// fetch entries
vector<COLEntry*>	COLFormat::getEntriesByModelName(string strModelName)
{
	vector<COLEntry*> vecCOLEntries;
	strModelName = String::toUpperCase(strModelName);
	for (auto pCOLEntry : getEntries())
	{
		if (String::toUpperCase(pCOLEntry->getModelName()) == strModelName)
		{
			vecCOLEntries.push_back(pCOLEntry);
		}
	}
	return vecCOLEntries;
}

// sort format
void				COLFormat::sort(void)
{
	std::sort(getEntries().begin(), getEntries().end(), sortFunction);
}

bool				COLFormat::sortFunction(COLEntry *pCOLEntry1, COLEntry *pCOLEntry2)
{
	return pCOLEntry1->getModelName() < pCOLEntry2->getModelName();
}