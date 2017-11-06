#pragma once

#include "nsbxgi.h"
#include <string>

struct bxgi::FormatVersion
{
	uint32					m_uiFileType;
	uint32					m_uiRawVersion;
	std::string				m_strVersionText;
};