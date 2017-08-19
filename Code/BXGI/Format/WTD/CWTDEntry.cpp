#include "CWTDEntry.h"
#include "Static/CString2.h"
#include "Image/CImageManager.h"
#include "Static/CDebug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CWTDEntry::CWTDEntry(void) :
	m_eD3DFormat(D3DFMT_UNKNOWN),
	m_eRasterDataFormat(bxcf::RASTERDATAFORMAT_UNKNOWN),
	m_uiRawDataOffset(0),
	m_uiTextureHash(0),
	m_ucLevels(0)
{
	m_usImageSize[0] = 0;
	m_usImageSize[1] = 0;
}

void					CWTDEntry::setRasterDataFormat(eRasterDataFormat eRasterDataFormatValue, bool bUpdateD3DFormat)
{
	m_eRasterDataFormat = eRasterDataFormatValue;
	if (bUpdateD3DFormat)
	{
		m_eD3DFormat = CImageManager::getD3DFormatFromRasterDataFormat(eRasterDataFormatValue);
	}
}

void					CWTDEntry::stripNameHeaderAndFooter(void)
{
	string strEntryName = getEntryName().substr(6);
	strEntryName = strEntryName.substr(0, strEntryName.length() - 4);
	setEntryName(strEntryName);
}