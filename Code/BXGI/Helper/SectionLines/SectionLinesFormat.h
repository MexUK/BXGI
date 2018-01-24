#pragma once

#include "nsbxgi.h"
#include "Format/ContainerFormat.h"
#include "Format/E2DFXType.h"
#include "Event/Events.h"
#include "../bxgi/Event/EEvent.h"
#include "Pool/VectorPool.h"
#include "Static/String.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
class bxgi::SectionLinesFormat : public bxcf::ContainerFormat
{
public:
	SectionLinesFormat(void);
	SectionLinesFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxcf::ContainerFormat(strFilePathOrData, bStringIsFilePath, false) {}

	virtual void										unload(void);

	std::map<SectionEnum, std::vector<EntryClass*>>&	getSectionEntries(void) { return m_umapSectionEntries; }

	uint32												getEntryCount(void) { return 20; } // todo

	void												unserializeLine(void);

	std::vector<std::string>							getModelNames(void);
	std::vector<std::string>							getTXDNames(void);
	std::vector<std::string>							getTXDNamesFromModelNames(std::vector<std::string>& vecModelNames);
	std::string											getTXDNameFromModelName(std::string strModelName);
	std::vector<EntryClass*>							getEntriesByModelName(std::string strModelName);
	std::vector<EntryClass*>							getEntriesByTXDName(std::string strTXDName);

	template<class EntryClass2>
	std::vector<EntryClass2*>							getEntriesBySection(SectionEnum eSectionValue);

	OtherEntryClass*									createOtherEntry(void);
	SectionEntryClass*									createSectionEntry(SectionEnum eFormatSectionValue);
	virtual DataEntryClass*								createDataEntry(SectionEnum eFormatSectionValue, uint32 uiSectionSpecificType = 0) = 0;

	virtual void										sortAllSectionsByObjectId(void);

	std::vector<std::vector<EntryClass*>>				getSectionsBySection(SectionEnum eFormatSectionValue);

	virtual SectionEnum									getSectionFromText(std::string strSectionText) = 0;
	virtual std::string									getSectionText(SectionEnum eSectionValue) = 0;

	virtual uint32										detectSectionSpecificType(SectionEnum eFormatSectionValue) = 0;

	// ContainerFormat

	bxcf::FormatEntry*									addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "") { return nullptr; } // todo
	bxcf::FormatEntry*									addEntryViaData(std::string& strEntryName, std::string& strEntryData) { return nullptr; }; // todo

	void												exportSingle(bxcf::FormatEntry* pEntry, std::string& strFolderPath);
	void												exportMultiple(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFolderPath) {} // todo
	void												exportAll(std::string& strFolderPath) {} // todo

	std::vector<bxcf::FormatEntry*>						getAllEntries(void);
	std::vector<bxcf::FormatEntry*>&					getEntriesRef(void);

	void												swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2) {} // todo

	uint32												getRawVersion(void) { return 0; } // todo

	void												merge(std::string& strFilePath) {} // todo
	void												split(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFilePathOut, uint32 uiFileVersionOut) {} // todo




	bxcf::VectorPool<EntryClass*>						getEntries(void) { return m_vecEntries; }

protected:
	void												unserializeText(void);
	void												serializeText(void);

private:
	void												setActiveReadSection(SectionEnum eActiveReadSection) { m_uiActiveReadSection = eActiveReadSection; }
	SectionEnum											getActiveReadSection(void) { return m_uiActiveReadSection; }

protected:
	bxcf::VectorPool<EntryClass*>						m_vecEntries;

private:
	std::map<SectionEnum, std::vector<EntryClass*>>		m_umapSectionEntries;
	SectionEnum											m_uiActiveReadSection;
};




template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::SectionLinesFormat(void) :
	ContainerFormat(false),
	m_uiActiveReadSection((SectionEnum)0)
{
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
void				bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::unload(void)
{
	for (auto it : m_umapSectionEntries)
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			pFormatEntry->unload();
			// todo - delete pFormatEntry;
		}
	}
	m_umapSectionEntries.clear();
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
void				bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::unserializeLine(void)
{
	// initialize
	EntryClass *pFormatEntry = nullptr;
	string strActiveLine = *m_reader.getActiveLine();

	// remove comment from end of line
	string strComment = "";
	size_t uiCommentPosition = strActiveLine.find('#');
	if (uiCommentPosition != string::npos)
	{
		strComment = strActiveLine.substr(uiCommentPosition + 1);
		strActiveLine = strActiveLine.substr(0, uiCommentPosition);
	}

	// trim line
	strActiveLine = String::trim(strActiveLine);

	// check if line is blank or section end
	if (strActiveLine == "" || String::toUpperCase(strActiveLine) == "END")
	{
		pFormatEntry = createOtherEntry();
		pFormatEntry->unserialize();
	}
	else
	{
		// set active line for data reader - keep blank lines intact (detected blank after comment removal and trimming)
		m_reader.setActiveLine(&strActiveLine);

		// check if line is a section name
		SectionEnum eFormatSectionValue = getSectionFromText(strActiveLine);
		if (eFormatSectionValue != 0) // 0 is the value of both IDE_SECTION_UNKNOWN and IPL_SECTION_UNKNOWN
		{
			pFormatEntry = createSectionEntry(eFormatSectionValue);
			pFormatEntry->unserialize();
			pFormatEntry->setEntryType2(SECTION_LINES_ENTRY_SECTION);
			setActiveReadSection(eFormatSectionValue);
		}
		else
		{
			// line is a data line
			m_reader.setTokenModeEnabled(true);
			m_reader.setLineTokens(String::split(String::replace(String::replace(strActiveLine, ",", " "), "\t", " "), " "));
			try
			{
				eFormatSectionValue = getActiveReadSection();
				pFormatEntry = createDataEntry(eFormatSectionValue, detectSectionSpecificType(eFormatSectionValue));
				pFormatEntry->unserialize();
				pFormatEntry->setEntryType2(SECTION_LINES_ENTRY_DATA);
			}
			catch (EExceptionCode)
			{
				// e.g. undocumeted formats: despite being detected as a data line, treat it as a other line to preserve data when serializing.
				if (pFormatEntry != nullptr)
				{
					delete pFormatEntry;
				}
				pFormatEntry = createOtherEntry();
				pFormatEntry->setSectionType(getActiveReadSection());
				pFormatEntry->unserialize();
				pFormatEntry->setEntryType2(SECTION_LINES_ENTRY_OTHER);
			}
			m_reader.setTokenModeEnabled(false);
		}
	}

	// apply comment to entry
	pFormatEntry->setComment(strComment);

	// store entry in Format object
	if (pFormatEntry->getEntryType() != SECTION_LINES_ENTRY_SECTION && pFormatEntry->getEntryType() != SECTION_LINES_ENTRY_OTHER)
	{
		//getSectionEntries().rbegin()->second.push_back(pFormatEntry);
		m_umapSectionEntries[pFormatEntry->getSectionType()].push_back(pFormatEntry);
	}
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
void				bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::unserializeText(void)
{
	m_reader.readAndStoreLines();
	while (m_reader.iterateLines())
	{
		bxcf::Events::trigger(bxgi::EEvent::UNSERIALIZE_FILE_PROGRESS, &m_reader);
		unserializeLine();
	}
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
void				bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::serializeText(void)
{
	DataWriter *pDataWriter = &m_writer;
	for (auto it : getSectionEntries())
	{
		pDataWriter->writeString(String::toLowerCase(getSectionText(it.first)));
		pDataWriter->writeString("\n");

		for (EntryClass *pFormatEntry : it.second)
		{
			pFormatEntry->serialize();
			pDataWriter->writeLineTokens();
			pDataWriter->writeString("\n");
		}

		pDataWriter->writeString("end");
		pDataWriter->writeString("\n");
	}
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<std::vector<EntryClass*>>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getSectionsBySection(SectionEnum eFormatSectionValue)
{
	std::vector<std::vector<EntryClass*>> vecFormatSections;
	for (auto it : getSectionEntries())
	{
		if (it.first == eFormatSectionValue)
		{
			vecFormatSections.push_back(it.second);
		}
	}
	return vecFormatSections;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
OtherEntryClass*	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::createOtherEntry(void)
{
	OtherEntryClass *pFormatEntry = new OtherEntryClass((FormatClass*)this);
	pFormatEntry->setEntryType2(SECTION_LINES_ENTRY_OTHER);
	return pFormatEntry;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
SectionEntryClass*	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::createSectionEntry(SectionEnum eFormatSectionValue)
{
	SectionEntryClass *pFormatEntry = new SectionEntryClass((FormatClass*)this);
	pFormatEntry->setEntryType2(SECTION_LINES_ENTRY_SECTION);
	pFormatEntry->setSectionType(eFormatSectionValue);
	return pFormatEntry;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<std::string>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getModelNames(void)
{
	std::vector<std::string> vecModelNames;
	for (auto it : getSectionEntries())
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			if (pFormatEntry->getEntryType() == SECTION_LINES_ENTRY_DATA)
			{
				std::string strModelName = String::toUpperCase(((DataEntryClass*)pFormatEntry)->getModelName());
				if (strModelName != "")
				{
					vecModelNames.push_back(strModelName);
				}
			}
		}
	}
	return vecModelNames;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<std::string>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getTXDNames(void)
{
	std::vector<std::string> vecTXDNames;
	for (auto it : getSectionEntries())
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			if (pFormatEntry->getEntryType() == SECTION_LINES_ENTRY_DATA)
			{
				std::string strTXDName = String::toUpperCase(((DataEntryClass*)pFormatEntry)->getTXDName());
				if (strTXDName != "")
				{
					vecTXDNames.push_back(strTXDName);
				}
			}
		}
	}
	return vecTXDNames;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<std::string>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getTXDNamesFromModelNames(std::vector<std::string>& vecModelNames)
{
	std::vector<std::string> vecTextureNames;
	for (auto strModelName : vecModelNames)
	{
		vecTextureNames.push_back(getTXDNameFromModelName(strModelName));
	}
	return vecTextureNames;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::string			bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getTXDNameFromModelName(std::string strModelName)
{
	strModelName = String::toUpperCase(strModelName);
	for (auto it : getSectionEntries())
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			if (pFormatEntry->getEntryType() == SECTION_LINES_ENTRY_DATA && strModelName == String::toUpperCase(((DataEntryClass*)pFormatEntry)->getModelName()))
			{
				return ((DataEntryClass*)pFormatEntry)->getTXDName();
			}
		}
	}
	return "";
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<EntryClass*>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getEntriesByModelName(std::string strModelName)
{
	strModelName = String::toUpperCase(strModelName);
	std::vector<EntryClass*> vecFormatEntries;
	for (auto it : getSectionEntries())
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			if (pFormatEntry->getEntryType() == SECTION_LINES_ENTRY_DATA && strModelName == String::toUpperCase(((DataEntryClass*)pFormatEntry)->getModelName()))
			{
				vecFormatEntries.push_back(pFormatEntry);
			}
		}
	}
	return vecFormatEntries;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<EntryClass*>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getEntriesByTXDName(std::string strTXDName)
{
	strTXDName = String::toUpperCase(strTXDName);
	std::vector<EntryClass*> vecFormatEntries;
	for (auto it : getSectionEntries())
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			if (pFormatEntry->getEntryType() == SECTION_LINES_ENTRY_DATA && strTXDName == String::toUpperCase(((DataEntryClass*)pFormatEntry)->getTXDName()))
			{
				vecFormatEntries.push_back(pFormatEntry);
			}
		}
	}
	return vecFormatEntries;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
template<class EntryClass2>
std::vector<EntryClass2*>	bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getEntriesBySection(SectionEnum eSectionValue)
{
	std::vector<EntryClass2*> vecDerivedEntries;
	for (auto it : getSectionEntries())
	{
		for (EntryClass *pFormatEntry : it.second)
		{
			if (pFormatEntry->getEntryType() == SECTION_LINES_ENTRY_DATA && it.first == eSectionValue)
			{
				vecDerivedEntries.push_back(((EntryClass2*)pFormatEntry));
			}
		}
	}
	return vecDerivedEntries;
}

template<class FormatClass, class EntryClass, class DataEntryClass>
bool						sortSectionLinesEntries_ObjectID(EntryClass *pFormatEntry1, EntryClass *pFormatEntry2) // todo - namespace
{
	// data lines
	if (pFormatEntry1->getEntryType() == bxgi::SECTION_LINES_ENTRY_DATA && pFormatEntry2->getEntryType() == bxgi::SECTION_LINES_ENTRY_DATA)
	{
		return ((DataEntryClass*)pFormatEntry1)->getObjectId() < ((DataEntryClass*)pFormatEntry2)->getObjectId();
	}

	// comments at top
	if (pFormatEntry1->getEntryType() == bxgi::SECTION_LINES_ENTRY_OTHER && pFormatEntry2->getEntryType() == bxgi::SECTION_LINES_ENTRY_OTHER)
	{
		return false;
	}
	else if (pFormatEntry1->getEntryType() == bxgi::SECTION_LINES_ENTRY_OTHER)
	{
		return true;
	}

	// fallback
	return false;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
void				bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::sortAllSectionsByObjectId(void)
{
	for (auto it : getSectionEntries())
	{
		std::sort(it.second.begin(), it.second.end(), sortSectionLinesEntries_ObjectID<FormatClass, EntryClass, DataEntryClass>);
	}
};

// ContainerFormat

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<bxcf::FormatEntry*>						bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getAllEntries(void)
{
	std::vector<bxcf::FormatEntry*> vecEntries;
	for (auto it : m_umapSectionEntries)
	{
		for (EntryClass *pIDEEntry : it.second)
		{
			vecEntries.push_back(pIDEEntry);
		}
	}
	return vecEntries;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
std::vector<bxcf::FormatEntry*>&					bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::getEntriesRef(void)
{
	return (std::vector<bxcf::FormatEntry*>&) m_vecEntries;
}

template<class FormatClass, class EntryClass, typename SectionEnum, class OtherEntryClass, class SectionEntryClass, class DataEntryClass>
void												bxgi::SectionLinesFormat<FormatClass, EntryClass, SectionEnum, OtherEntryClass, SectionEntryClass, DataEntryClass>::exportSingle(bxcf::FormatEntry* pEntry, std::string& strFolderPath)
{
	bxgi::SectionLinesEntry<FormatClass, SectionEnum> *pLineEntry = (bxgi::SectionLinesEntry<FormatClass, SectionEnum>*) pEntry;
	bxcf::String::setClipboardText(pLineEntry->getEntryData());
}