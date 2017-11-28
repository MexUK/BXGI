#pragma once

#include "nsbxgi.h"
#include "EDATLoaderEntryType.h"
#include "Format/FormatEntry.h"
#include "Format/EFileType.h"
#include <string>
#include <deque>

class bxgi::DATLoaderEntry : public bxcf::FormatEntry
{
public:
	DATLoaderEntry(DATLoaderFormat *pFormat);

	void						unload(void) {}

	void						serialize(void);

	std::string					getEntryTypeText(void);

	void						setFormat(DATLoaderFormat *pFormat) { m_pFormat = pFormat; }
	DATLoaderFormat*			getFormat(void) { return m_pFormat; }

	void						setEntryType(EDATLoaderEntryType eEntryType) { m_uiEntryType = eEntryType; }
	EDATLoaderEntryType			getEntryType(void) { return m_uiEntryType; }

	void						setEntryValues(std::deque<std::string>& deqValues) { m_deqValues = deqValues; }
	std::deque<std::string>&	getEntryValues(void) { return m_deqValues; }

	// FormatEntry
	void						setEntryName(std::string& strEntryName) {}
	std::string&				getEntryName(void) { return bxcf::g_strBlankString; }

	void						replace(std::string& strFilePath) {}

	uint32						getIndex(void) { return 0; }
	std::string					getEntryExtension(void) { return ""; }
	uint32						getEntryOffset(void) { return 0; }
	uint32						getEntrySize(void) { return 0; }
	uint32						getRawVersion(void) { return 0; }
	std::string					getVersionText(void) { return ""; }

	std::string					getEntryData(void) { return ""; }

	bxcf::fileType::EFileType	getFileType(void) { return bxcf::fileType::EFileType::DAT_LOADER; } // todo rename to getEntryType

	// DATLoaderEntry continued

private:
	DATLoaderFormat*			m_pFormat;
	EDATLoaderEntryType			m_uiEntryType;
	std::deque<std::string>		m_deqValues;
};