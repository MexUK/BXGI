#include "RWEntry_TextureNative_MipMap.h"
#include "Format/RW/Sections/RWSection_TextureNative.h"
#include "Static/String.h"
#include "Image/ImageManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

string				RWEntry_TextureNative_MipMap::getRasterDataBGRA32(void)
{
	if (getRasterDataFormat() == RASTERDATAFORMAT_BGRA32)
	{
		// the mipmap's raster data is currently stored in format BGRA32
		return m_strRasterData;
	}
	else
	{
		// the mipmap's raster data is currently stored in a format that is not BGRA32
		return ImageManager::convertRasterDataFormatToBGRA32(m_strRasterData, getRasterDataFormat(), getTexture()->getPaletteData(), getImageSize().x, getImageSize().y);
	}
}

string				RWEntry_TextureNative_MipMap::getRasterDataRGBA32(void)
{
	return ImageManager::convertBGRA32ToRGBA32(getRasterDataBGRA32());
}

void				RWEntry_TextureNative_MipMap::convertToRasterDataFormat(ERasterDataFormat ERasterDataFormatValue)
{
	if (getRasterDataFormat() == ERasterDataFormatValue)
	{
		// raster data is already in the requested format
		return;
	}

	// convert raster data to requested format
	string strPaletteData = "";
	setRasterData(ImageManager::convertBGRA32ToRasterDataFormat(getRasterDataBGRA32(), ERasterDataFormatValue, &strPaletteData, getImageSize().x, getImageSize().y));
}

bool				RWEntry_TextureNative_MipMap::canRasterDataBeConverted(void)
{
	if (getImageSize().x < 4 || getImageSize().y < 4)
	{
		return false;
	}

	return true;
}

ERasterDataFormat	RWEntry_TextureNative_MipMap::getRasterDataFormat(void)
{
	return getTexture()->getRasterDataFormat();
}