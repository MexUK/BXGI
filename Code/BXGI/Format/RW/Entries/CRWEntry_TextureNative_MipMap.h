#pragma once

#include "bxgi.h"
#include "Type/Types.h"
#include "Type/Vector/Vec2u.h"
#include "Image/eRasterDataFormat.h"
#include <string>

class bxgi::CRWSection_TextureNative;

class bxgi::CRWEntry_TextureNative_MipMap
{
public:
	CRWEntry_TextureNative_MipMap(CRWSection_TextureNative *pTextureNative) :
		m_pTextureNative(pTextureNative)
	{
		m_vecImageSize.x = 0;
		m_vecImageSize.y = 0;
		m_vecSwizzledImageSize.x = 0;
		m_vecSwizzledImageSize.y = 0;
	};

	void						unload(void) {}

	bool						canRasterDataBeConverted(void);
	bxcf::eRasterDataFormat			getRasterDataFormat(void);
	std::string					getRasterDataBGRA32(void);
	std::string					getRasterDataRGBA32(void);

	void						convertToRasterDataFormat(bxcf::eRasterDataFormat eRasterDataFormatValue);

	void						setRasterData(std::string& strData) { m_strRasterData = strData; }
	std::string&				getRasterData(void) { return m_strRasterData; }

	void						setImageSize(bxcf::Vec2u& vecImageSize) { m_vecImageSize = vecImageSize; }
	bxcf::Vec2u&					getImageSize(void) { return m_vecImageSize; }

	void						setSwizzledImageSize(bxcf::Vec2u& vecSwizzledImageSize) { m_vecSwizzledImageSize = vecSwizzledImageSize; }
	bxcf::Vec2u&					getSwizzledImageSize(void) { return m_vecSwizzledImageSize; }

	void						setTexture(CRWSection_TextureNative *pTextureNative) { m_pTextureNative = pTextureNative; }
	CRWSection_TextureNative*	getTexture(void) { return m_pTextureNative; }

private:
	CRWSection_TextureNative*	m_pTextureNative;
	bxcf::Vec2u						m_vecImageSize;
	bxcf::Vec2u						m_vecSwizzledImageSize;
	std::string					m_strRasterData;
};