#ifndef CRWSection_Light_H
#define CRWSection_Light_H

#include "bxgi.h"
#include "Engine/RW/CRWSection.h"
#include <string>

class bxgi::CRWSection_Light : public bxgi::CRWSection
{
public:
	CRWSection_Light(void);

	void							unserialize(void);
	void							serialize(void);

	void							setData(std::string& strData) { m_strData = strData; }
	std::string&					getData(void) { return m_strData; }

private:
	std::string						m_strData;
};

#endif