#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"

#pragma pack(push, 1)
struct bxgi::RG_IMGEntry_Version2
{
	uint32			m_uiOffsetInSectors;
	uint16			m_uiStreamingSizeInSectors;
	uint16			m_uiArchiveSizeInSectors;
	uint8			m_szName[24];
};
#pragma pack(pop)