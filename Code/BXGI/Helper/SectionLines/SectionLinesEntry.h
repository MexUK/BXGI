#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "ESectionLinesEntryType.h"
#include "Format/FormatEntry.h"
#include <string>

template<class FormatClass, typename SectionEnum>
class bxgi::SectionLinesEntry : public bxcf::FormatEntry
{
public:
	SectionLinesEntry(FormatClass *pFormat) :
		m_pFormat(pFormat),
		m_uiEntryType2(SECTION_LINES_ENTRY_UNKNOWN),
		m_uiSectionType((SectionEnum)0),
		m_ucFormatType(0)
	{
	}

	virtual void					unload(void) {}

	virtual void					unserialize(void) = 0;
	virtual void					serialize(void) = 0;

	void							setFormat(FormatClass* pFormat) { m_pFormat = pFormat; }
	FormatClass*					getFormat(void) { return m_pFormat; }

	void							setEntryType2(bxgi::ESectionLinesEntryType eEntryType) { m_uiEntryType2 = eEntryType; }
	bxgi::ESectionLinesEntryType	getEntryType2(void) { return m_uiEntryType2; }

	void							setSectionType(SectionEnum eSectionValue) { m_uiSectionType = eSectionValue; }
	SectionEnum						getSectionType(void) { return m_uiSectionType; }

	void							setComment(std::string& strComment) { m_strComment = strComment; }
	std::string&					getComment(void) { return m_strComment; }

	void							setFormatType(uint8 ucFormatType) { m_ucFormatType = ucFormatType; }
	uint8							getFormatType(void) { return m_ucFormatType; }

	// FormatEntry

	void							setEntryName(std::string& strEntryName) { } // dynamic length - todo
	inline std::string&				getEntryName(void) { return bxcf::g_strBlankString; } // dynamic length - todo

	uint32							getIndex(void) { return -1; } // todo
	inline std::string				getEntryExtension(void) { return "a"; } // todo
	inline uint32					getEntryOffset(void) { return 0; } // in bytes - todo
	inline uint32					getEntrySize(void) { return 0; } // in bytes - todo
	inline uint32					getRawVersion(void) { return 0; } // todo
	std::string						getVersionText(void) { return "b"; } // todo

	void							replace(std::string& strFilePath) {} // todo

	std::string						getEntryData(void) { return ""; } // todo

protected:
	FormatClass*					m_pFormat;

private:
	bxgi::ESectionLinesEntryType	m_uiEntryType2;
	SectionEnum						m_uiSectionType;
	std::string						m_strComment;
	uint8							m_ucFormatType;
};