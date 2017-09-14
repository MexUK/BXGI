#pragma once

#include "nsbxgi.h"
#include "Format/RW/RWSection.h"
#include <string>

class bxgi::RWSection_String : public bxgi::RWSection
{
public:
	RWSection_String(void);

	void							_unserialize(void);
	void							_serialize(void);

	void							setData(std::string& strData) { m_strData = strData; }
	std::string&					getData(void) { return m_strData; }

private:
	std::string						m_strData;
};