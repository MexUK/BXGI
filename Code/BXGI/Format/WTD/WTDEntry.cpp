#include "WTDEntry.h"
#include "Static/String.h"
#include "Image/ImageManager.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

WTDEntry::WTDEntry(void) :
	m_uiD3DFormat(D3DFMT_UNKNOWN),
	m_uiRasterDataFormat(bxcf::RASTERDATAFORMAT_UNKNOWN),
	m_uiRawDataOffset(0),
	m_uiTextureHash(0),
	m_ucLevels(0)
{
	m_usImageSize[0] = 0;
	m_usImageSize[1] = 0;
}

void					WTDEntry::replace(string& strFilePath)
{
	// todo
}

uint32					WTDEntry::getIndex(void)
{
	// return m_pFile->getIndexByEntry(this);
	// todo
	return 0;
}

string					WTDEntry::getEntryExtension(void)
{
	// todo
	return "a";
}

uint32					WTDEntry::getEntryOffset(void)
{
	// todo
	return 0;
}

uint32					WTDEntry::getEntrySize(void)
{
	// todo
	return 0;
}

string					WTDEntry::getVersionText(void)
{
	// todo
	return "b";
}

void					WTDEntry::setRasterDataFormat(ERasterDataFormat ERasterDataFormatValue, bool bUpdateD3DFormat)
{
	m_uiRasterDataFormat = ERasterDataFormatValue;
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