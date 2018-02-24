#include "COLFormat.h"
#include "Static/String.h"
#include "Static/File.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "COLManager.h"
#include "Static/Path.h"
#include "Static/Debug.h"
#include "Event/Events.h"
#include "Event/EEvent.h"
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
	checkToFixPEDSDotCOL();

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
		Events::triggerDefault(UNSERIALIZE_FILE_PROGRESS, &m_reader);
	}
}

void				COLFormat::_serialize(void)
{
	for (COLEntry *pCOLEntry : getEntries())
	{
		pCOLEntry->serialize();
	}
}

void				COLFormat::checkToFixPEDSDotCOL(void)
{
	// thanks to: http://gtaforums.com/topic/809134-restoring-the-pedscol/

	if (String::toUpperCase(Path::getFileName(m_reader.m_strFilePath)) != "PEDS.COL")
	{
		return;
	}

	m_reader.setData(m_reader.readAll());

	// fix data

	// corrupt collision: cop
	m_reader.getData()[0x190] = 0;
	m_reader.getData()[0x258] = 4;
	m_reader.getData()[0x2BC] = 6;

	// broken collision: playert
	m_reader.getData()[0x578] = 6;
	m_reader.getData()[0x5DC] = 0x0F;

	// broken collision: mwat
	m_reader.getData()[0x898] = 0;
	m_reader.getData()[0x8FC] = 8;

	// corrupt collision: b_wom1
	m_reader.getData().insert(0x4955, 1, 78);

	// corrupt collision: fatmale02
	m_reader.getData().insert(0x2094, 1, 0);

	// corrupt collision: male01
	m_reader.getData().insert(0x155F, 1, 78);
}

// COL version
void				COLFormat::setCOLVersion(ECOLVersion uiCOLVersion)
{
	for (COLEntry *pCOLEntry : getEntries())
	{
		pCOLEntry->setCOLVersion(uiCOLVersion);
	}
}

// model names
vector<string>		COLFormat::getModelNames(void)
{
	vector<string> vecModelNames;
	for (COLEntry *pCOLEntry : getEntries())
	{
		vecModelNames.push_back(pCOLEntry->getModelName());
	}
	return vecModelNames;
}

// fetch entry
COLEntry*			COLFormat::getEntryByName(string strName)
{
	strName = String::toUpperCase(strName);
	for (COLEntry *pCOLEntry : getEntries())
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
	for (COLEntry *pCOLEntry : getEntries())
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