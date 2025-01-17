#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Helper/SectionLines/SectionLinesFormat.h"
//#include "eDataType.h" // todo - no idea why this include is needed - it makes EIDESection underlined when not here
#include "EIDESection.h"
#include "Format/E2DFXType.h"
#include "EIDEPathType.h"
#include "Entry/IDEEntry_Other.h"
#include "Entry/IDEEntry_Section.h"
#include "Entry/IDEEntry_Data.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_2DFX.h"
#include <string>
#include <vector>

class bxgi::IDEFormat;
class bxgi::IDEEntry;

class bxgi::IDEFormat : public bxgi::SectionLinesFormat<bxgi::IDEFormat, bxgi::IDEEntry, bxgi::EIDESection, bxgi::IDEEntry_Other, bxgi::IDEEntry_Section, bxgi::IDEEntry_Data>
{
public:
	IDEFormat(void);
	IDEFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxgi::SectionLinesFormat<bxgi::IDEFormat, bxgi::IDEEntry, bxgi::EIDESection, bxgi::IDEEntry_Other, bxgi::IDEEntry_Section, bxgi::IDEEntry_Data>(strFilePathOrData, bStringIsFilePath) {}

	bxgi::IDEEntry_Data*						createDataEntry(EIDESection EIDESectionValue, uint32 uiSectionSpecificType = 0);
	
	template<class EntryClass2>
	std::vector<EntryClass2*>					getEntriesBySection(EIDESection EIDESectionValue, bxgi::E2DFXType E2DFXTypeValue = bxgi::_2DFX_UNKNOWN);

	std::vector<std::string>					getModelNamesInSections(std::vector<EIDESection>& vecIDESections);
	std::vector<std::string>					getTextureSetNamesInSections(std::vector<EIDESection>& vecIDESections);

	EIDESection									getSectionFromText(std::string strIDESectionText);
	std::string									getSectionText(EIDESection EIDESectionValue);

	uint32										detectSectionSpecificType(EIDESection EIDESectionValue);

	uint32										getFormatGames(void);

	// ContainerFormat virtuals
	bxgi::IDEEntry*								addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	bxgi::IDEEntry*								addEntryViaData(std::string& strEntryName, std::string& strEntryData);

	void										exportSingle(bxcf::FormatEntry *pEntry, std::string& strFolderPath) {}
	void										exportMultiple(std::vector<bxcf::FormatEntry*>& vecIMGEntries, std::string& strFolderPath) {}
	void										exportAll(std::string& strFolderPath) {}

	std::vector<bxcf::FormatEntry*>				getAllEntries(void) { return (std::vector<bxcf::FormatEntry*>)(std::vector<bxcf::FormatEntry*>&)m_vecEntries; }
	std::vector<bxcf::FormatEntry*>&			getEntriesRef(void) { return (std::vector<bxcf::FormatEntry*>&)m_vecEntries; }

	void										swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2) {}

	uint32										getRawVersion(void) { return -1; }

	void										merge(std::string& strFilePath);
	void										split(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFilePathOut, uint32 uiFileVersionOut) {}

	// IDEFormat continued
	void										mergeViaData(std::string& strFileData);

private:
	void										_unserialize(void);
	void										_serialize(void);

	bxgi::IDEEntry_Data*						unserializeDataEntry(EIDESection EIDESectionValue);

	std::pair<bxgi::E2DFXType, uint32>			detect2DFXTypeAndGame(void);
	EIDEPathType								detectPATHType(void);
};

template<class EntryClass2>
std::vector<EntryClass2*>						bxgi::IDEFormat::getEntriesBySection(EIDESection EIDESectionValue, bxgi::E2DFXType E2DFXTypeValue)
{
	if (EIDESectionValue != IDE_SECTION_2DFX || E2DFXTypeValue == bxgi::_2DFX_UNKNOWN)
	{
		return SectionLinesFormat::getEntriesBySection<EntryClass2>(EIDESectionValue);
	}
	else
	{
		std::vector<EntryClass2*> vecDerivedEntries;
		for (EntryClass2* pIDEEntry : SectionLinesFormat::getEntriesBySection<EntryClass2>(EIDESectionValue))
		{
			if (((bxgi::IDEEntry_2DFX*)pIDEEntry)->get2DFXType() == E2DFXTypeValue)
			{
				vecDerivedEntries.push_back(pIDEEntry);
			}
		}
		return vecDerivedEntries;
	}
}