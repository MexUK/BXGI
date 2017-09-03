#include "WTDEntry.h"
#include "Static/String2.h"
#include "Image/ImageManager.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

WTDEntry::WTDEntry(void) :
	m_uiD3DFormat(D3DFMT_UNKNOWN),
	m_ERasterDataFormat(bxcf::RASTERDATAFORMAT_UNKNOWN),
	m_uiRawDataOffset(0),
	m_uiTextureHash(0),
	m_ucLevels(0)
{
	m_usImageSize[0] = 0;
	m_usImageSize[1] = 0;
}

void					WTDEntry::setRasterDataFormat(ERasterDataFormat ERasterDataFormatValue, bool bUpdateD3DFormat)
{
	m_ERasterDataFormat = ERasterDataFormatValue;
	if (bUpdateD3DFormat)
	{
		m_uiD3DFormat = ImageManager::getD3DFormatFromRasterDataFormat(ERasterDataFormatValue);
	}
}

void					WTDEntry::stripNameHeaderAndFooter(void)
{
	string strEntryName = getEntryName().substr(6);
	strEntryName = strEntryName.substr(0, strEntryName.length() - 4);
	setEntryName(strEntryName);
}