#include "CWTDManager.h"
#include "CWTDEntry.h"
#include "Static/CFile.h"
#include "Static/CString2.h"
//#include "CIMGManager.h" // WTD Manager project only relies upon IMG Manager project for it's decompressZLib function.
#include "Static/CDebug.h"
#include "CWTDMipmap.h"
#include "Intermediate/Texture/CIntermediateTextureFormat.h"
#include "Intermediate/Texture/CIntermediateTexture.h"
#include "Intermediate/Texture/Data/CIntermediateTextureMipmap.h"
#include "Image/CImageManager.h"
#include "Compression/CCompressionManager.h"
#include "Image/eRasterDataFormat.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CWTDManager::CWTDManager(void)
{
}

void					CWTDManager::init(void)
{
}
void					CWTDManager::uninit(void)
{
}

uint32			CWTDManager::getImageDataSize(CWTDEntry *pWTDEntry, bool bIncludeLevels)
{
	uint32 uiImageDataSize = 0;
	switch (pWTDEntry->getD3DFormat())
	{
	case D3DFMT_DXT1:
		uiImageDataSize = pWTDEntry->getImageSize(true) * pWTDEntry->getImageSize(false) / 2;
		break;
	case D3DFMT_DXT3:
	case D3DFMT_DXT5:
		uiImageDataSize = pWTDEntry->getImageSize(true) * pWTDEntry->getImageSize(false);
		break;
	}

	if (bIncludeLevels)
	{
		int levels = pWTDEntry->getLevels();
		int levelDataSize = uiImageDataSize;
		while (levels > 1)
		{
			uiImageDataSize += (levelDataSize / 4);

			levelDataSize /= 4;

			// clamp to 16 bytes
			if (levelDataSize < 16)
			{
				if (pWTDEntry->getD3DFormat() == D3DFMT_DXT1 && levelDataSize < 8)
				{
					levelDataSize = 8;
				}
				else
				{
					levelDataSize = 16;
				}
			}

			levels--;
		}
	}

	return uiImageDataSize;
}

CWTDFormat*				CWTDManager::convertIntermediateTextureFileToWTDFile(bxgi::CIntermediateTextureFormat *pIntermediateTextureFormat)
{
	CWTDFormat *pWTDFile = new CWTDFormat;

	for (CIntermediateTexture *pGeneralTexture : pIntermediateTextureFormat->getEntries())
	{
		CWTDEntry *pWTDEntry = new CWTDEntry;

		pWTDEntry->setEntryName(pGeneralTexture->getName());
		pWTDEntry->setD3DFormat(CImageManager::getD3DFormatFromRasterDataFormat(pGeneralTexture->getRasterDataFormat()));
		pWTDEntry->setImageSize(true, pGeneralTexture->getSize().x);
		pWTDEntry->setImageSize(false, pGeneralTexture->getSize().y);
		
		for (CIntermediateTextureMipmap *pGeneralMipmap : pGeneralTexture->getEntries())
		{
			CWTDMipmap *pWTDMipmap = new CWTDMipmap(pWTDEntry);

			pWTDMipmap->setRasterData(pGeneralMipmap->getRasterData());
			pWTDMipmap->setImageSize(true, pGeneralMipmap->getSize().x);
			pWTDMipmap->setImageSize(false, pGeneralMipmap->getSize().y);

			pWTDEntry->addEntry(pWTDMipmap);
		}

		pWTDFile->addEntry(pWTDEntry);
	}

	return pWTDFile;
}