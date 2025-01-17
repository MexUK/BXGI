#pragma once

#include "nsbxgi.h"
#include "Format/RW/RWSection.h"
#include <string>

class bxgi::RWSection_UnknownSection : public bxgi::RWSection
{
public:
	RWSection_UnknownSection(void);

	void							_unserialize(void);
	void							_serialize(void);

	void							setActualSectionId(uint32 uiActualSectionId) { m_uiActualSectionId = uiActualSectionId; }
	uint32							getActualSectionId(void) { return m_uiActualSectionId; }

	void							setData(std::string& strData) { m_strData = strData; }
	std::string&					getData(void) { return m_strData; }

private:
	uint32							m_uiActualSectionId;
	std::string						m_strData;
};