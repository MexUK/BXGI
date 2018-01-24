#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "EIDESection.h"
#include "Helper/SectionLines/SectionLinesEntry.h"
#include "nsbxcf.h"
#include "Format/EFIleType.h"
#include <string>

class bxgi::IDEFormat;

class bxgi::IDEEntry : public bxgi::SectionLinesEntry<IDEFormat, EIDESection>
{
public:
	IDEEntry(IDEFormat *pIDEFormat);

	// FormatEntry
	void									setEntryName(std::string& strEntryName);
	std::string&							getEntryName(void);

	void									replace(std::string& strFilePath);

	uint32									getIndex(void);
	std::string								getEntryExtension(void);
	uint32									getEntryOffset(void);
	uint32									getEntrySize(void);
	uint32									getRawVersion(void);
	std::string								getVersionText(void);

	std::string								getEntryData(void);

	inline bxcf::fileType::EFileType		getEntryType(void) { return bxcf::fileType::EFileType::ITEM_DEFINITION; }

	// IDEEntry continued
	std::string								serializeData(void);
};