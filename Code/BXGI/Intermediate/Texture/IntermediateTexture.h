#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Pool/VectorPool.h"
#include "Image/ERasterDataFormat.h"
#include "Data/IntermediateTextureMipmap.h"
#include "Type/Vector/Vec2u16.h"
#include <string>

class bxgi::IntermediateTexture : public bxcf::VectorPool<bxgi::IntermediateTextureMipmap*>
{
public:
	IntermediateTexture(void) :
		m_uiRasterDataFormat(bxcf::RASTERDATAFORMAT_UNKNOWN)
	{
		m_vecSize.x = 0;
		m_vecSize.y = 0;
	};

	void						unload(void) {}

	void						setName(std::string& strName) { m_strName = strName; }
	std::string&				getName(void) { return m_strName; }

	void						setRasterDataFormat(bxcf::ERasterDataFormat ERasterDataFormatValue) { m_uiRasterDataFormat = ERasterDataFormatValue; }
	bxcf::ERasterDataFormat		getRasterDataFormat(void) { return m_uiRasterDataFormat; }

	void						setSize(bxcf::Vec2u16& vecSize) { m_vecSize = vecSize; }
	bxcf::Vec2u16&				getSize(void) { return m_vecSize; }

	void						setPaletteData(std::string& strPaletteData) { m_strPaletteData = strPaletteData; }
	std::string&				getPaletteData(void) { return m_strPaletteData; }

private:
	std::string					m_strName;
	bxcf::ERasterDataFormat		m_uiRasterDataFormat;
	bxcf::Vec2u16				m_vecSize;
	std::string					m_strPaletteData;
};