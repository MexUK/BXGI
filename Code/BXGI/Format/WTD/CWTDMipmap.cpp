#include "CWTDMipmap.h"
#include "CWTDEntry.h"
#include "Image/ImageManager.h"
//#include "squish.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CWTDMipmap::CWTDMipmap(CWTDEntry *pWTDEntry) :
	m_pWTDEntry(pWTDEntry)
{
	m_usImageSize[0] = 0;
	m_usImageSize[1] = 0;
}

string					CWTDMipmap::getRasterDataBGRA32(void)
{
	if (getRasterDataFormat() == RASTERDATAFORMAT_BGRA32)
	{
		// the mipmap's raster data is currently stored in format BGRA32
		return m_strRasterData;
	}
	else
	{
		// the mipmap's raster data is currently stored in a format that is not BGRA32
		string strPaletteData = "";
		return ImageManager::convertRasterDataFormatToBGRA32(m_strRasterData, getRasterDataFormat(), strPaletteData, getImageSize(true), getImageSize(false));
	}
}

ERasterDataFormat		CWTDMipmap::getRasterDataFormat(void)
{
	return getWTDEntry()->getRasterDataFormat();
}
