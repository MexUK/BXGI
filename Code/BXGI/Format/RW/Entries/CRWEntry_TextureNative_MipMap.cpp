#include "CRWEntry_TextureNative_MipMap.h"
#include "Format/RW/Sections/CRWSection_TextureNative.h"
#include "Static/CString2.h"
#include "Image/CImageManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

string				CRWEntry_TextureNative_MipMap::getRasterDataBGRA32(void)
{
	if (getRasterDataFormat() == RASTERDATAFORMAT_BGRA32)
	{
		// the mipmap's raster data is currently stored in format BGRA32
		return m_strRasterData;
	}
	else
	{
		// the mipmap's raster data is currently stored in a format that is not BGRA32
		return CImageManager::convertRasterDataFormatToBGRA32(m_strRasterData, getRasterDataFormat(), getTexture()->getPaletteData(), getImageSize().x, getImageSize().y);
	}
}

string				CRWEntry_TextureNative_MipMap::getRasterDataRGBA32(void)
{
	return CImageManager::convertBGRA32ToRGBA32(getRasterDataBGRA32());
}

void				CRWEntry_TextureNative_MipMap::convertToRasterDataFormat(eRasterDataFormat eRasterDataFormatValue)
{
	if (getRasterDataFormat() == eRasterDataFormatValue)
	{
		// raster data is already in the requested format
		return;
	}

	// convert raster data to requested format
	string strPaletteData = "";
	setRasterData(CImageManager::convertBGRA32ToRasterDataFormat(getRasterDataBGRA32(), eRasterDataFormatValue, &strPaletteData, getImageSize().x, getImageSize().y));
}

bool				CRWEntry_TextureNative_MipMap::canRasterDataBeConverted(void)
{
	if (getImageSize().x < 4 || getImageSize().y < 4)
	{
		return false;
	}

	return true;
}

eRasterDataFormat	CRWEntry_TextureNative_MipMap::getRasterDataFormat(void)
{
	return getTexture()->getRasterDataFormat();
}