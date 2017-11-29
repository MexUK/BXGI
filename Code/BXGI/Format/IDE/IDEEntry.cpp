#include "IDEEntry.h"
#include "IDEFormat.h"
#include "Entry/IDEEntry_Data.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IDEEntry::IDEEntry(IDEFormat *pIDEFormat) :
	SectionLinesEntry(pIDEFormat)
{
}

// FormatEntry
void									IDEEntry::setEntryName(string& strEntryName)
{
	if (getEntryType() != SECTION_LINES_ENTRY_DATA)
	{
		return;
	}

	IDEEntry_Data *pIDEDataEntry = (IDEEntry_Data *)this;
	pIDEDataEntry->setModelName(strEntryName);
}

string&									IDEEntry::getEntryName(void)
{
	if (getEntryType() != SECTION_LINES_ENTRY_DATA)
	{
		return g_strBlankString;
	}

	IDEEntry_Data *pIDEDataEntry = (IDEEntry_Data *)this;
	return pIDEDataEntry->getModelName();
}

void									IDEEntry::replace(string& strFilePath)
{
}

uint32									IDEEntry::getIndex(void)
{
	return m_pFormat->getEntries().getIndexByEntry(this);
}

string									IDEEntry::getEntryExtension(void)
{
	return "";
}

uint32									IDEEntry::getEntryOffset(void)
{
	return getIndex();
}

uint32									IDEEntry::getEntrySize(void)
{
	return serializeData().length();
}

uint32									IDEEntry::getRawVersion(void)
{
	if (getEntryType() != SECTION_LINES_ENTRY_DATA)
	{
		return -1;
	}

	IDEEntry_Data *pIDEDataEntry = (IDEEntry_Data *)this;
	return pIDEDataEntry->getFormatType();
}

string									IDEEntry::getVersionText(void)
{
	if (getEntryType() != SECTION_LINES_ENTRY_DATA)
	{
		return "";
	}

	IDEEntry_Data *pIDEDataEntry = (IDEEntry_Data *)this;
	return pIDEDataEntry->getVersionText();
}

string									IDEEntry::getEntryData(void)
{
	return serializeData();
}

// IDEEntry continued
string									IDEEntry::serializeData(void)
{
	if (getEntryType() != SECTION_LINES_ENTRY_DATA)
	{
		return "";
	}

	DataWriter *pDataWriter = &m_pFormat->m_writer;
	IDEEntry_Data *pIDEDataEntry = (IDEEntry_Data *)this;

	pDataWriter->setStreamType(DATA_STREAM_MEMORY);
	pIDEDataEntry->serialize();
	string strData = pDataWriter->getData();
	pDataWriter->reset();
	return strData;
}