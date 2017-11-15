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

	inline bxcf::fileType::EFileType	getFileType(void) { return bxcf::fileType::EFileType::ITEM_DEFINITION; }
};