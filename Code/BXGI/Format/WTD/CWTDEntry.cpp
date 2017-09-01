#include "CWTDEntry.h"
#include "Static/CString2.h"
#include "Image/CImageManager.h"
#include "Static/CDebug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CWTDEntry::CWTDEntry(void) :
	m_uiD3DFormat(D3DFMT_UNKNOWN),
	m_ERasterDataFormat(bxcf::RASTERDATAFORMAT_UNKNOWN),
	m_uiRawDataOffset(0),
	m_uiTextureHash(0),
	m_ucLevels(0)
{
	m_usImageSize[0] = 0;
	m_usImageSize[1] = 0;
}

void					CWTDEntry::setRasterDataFormat(ERasterDataFormat ERasterDataFormatValue, bool bUpdateD3DFormat)
{
	m_ERasterDataFormat = ERasterDataFormatValue;
	if (bUpdateD3DFormat)
	{
		m_uiD3DFormat = CImageManager::getD3DFormatFromRasterDataFormat(ERasterDataFormatValue);
	}
}

void					CWTDEntry::stripNameHeaderAndFooter(void)
{
	string strEntryName = getEntryName().substr(6);
	strEntryName = strEntryName.substr(0, strEntryName.length() - 4);
	setEntryName(strEntryName);
}