#include "WTDEntry.h"
#include "Static/String.h"
#include "Image/ImageManager.h"
#include "Image/ImageFile.h"
#include "Format/WTD/WTDMipmap.h"
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
	ImageFile *pImageFile = ImageManager::loadImageFromFile(strFilePath);
	if (!pImageFile)
	{
		return;
	}

	setImageSize(true, pImageFile->m_uiImageWidth);
	setImageSize(false, pImageFile->m_uiImageHeight);
	setRasterDataFormat(RASTERDATAFORMAT_BGRA32);

	removeAllEntries();

	WTDMipmap *pMipMap = new WTDMipmap(this);
	pMipMap->setImageSize(true, pImageFile->m_uiImageWidth);
	pMipMap->setImageSize(false, pImageFile->m_uiImageHeight);
	pMipMap->setRasterData(pImageFile->m_strRasterDataBGRA32);
	addEntry(pMipMap);
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
	if (getEntryName() == "")
	{
		return;
	}

	string strEntryName = getEntryName().substr(6);
	strEntryName = strEntryName.substr(0, strEntryName.length() - 4);
	setEntryName(strEntryName);
}