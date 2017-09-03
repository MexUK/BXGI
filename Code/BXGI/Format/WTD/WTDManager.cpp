#include "WTDManager.h"
#include "WTDEntry.h"
#include "Static/File.h"
#include "Static/String2.h"
//#include "IMGManager.h" // WTD Manager project only relies upon IMG Manager project for it's decompressZLib function.
#include "Static/Debug.h"
#include "WTDMipmap.h"
#include "Intermediate/Texture/IntermediateTextureFormat.h"
#include "Intermediate/Texture/IntermediateTexture.h"
#include "Intermediate/Texture/Data/IntermediateTextureMipmap.h"
#include "Image/ImageManager.h"
#include "Compression/CompressionManager.h"
#include "Image/ERasterDataFormat.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

WTDManager::WTDManager(void)
{
}

void					WTDManager::init(void)
{
}
void					WTDManager::uninit(void)
{
}

uint32			WTDManager::getImageDataSize(WTDEntry *pWTDEntry, bool bIncludeLevels)
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

WTDFormat*				WTDManager::convertIntermediateTextureFileToWTDFile(IntermediateTextureFormat *pIntermediateTextureFormat)
{
	WTDFormat *pWTDFile = new WTDFormat;

	for (IntermediateTexture *pGeneralTexture : pIntermediateTextureFormat->getEntries())
	{
		WTDEntry *pWTDEntry = new WTDEntry;

		pWTDEntry->setEntryName(pGeneralTexture->getName());
		pWTDEntry->setD3DFormat(ImageManager::getD3DFormatFromRasterDataFormat(pGeneralTexture->getRasterDataFormat()));
		pWTDEntry->setImageSize(true, pGeneralTexture->getSize().x);
		pWTDEntry->setImageSize(false, pGeneralTexture->getSize().y);
		
		for (IntermediateTextureMipmap *pGeneralMipmap : pGeneralTexture->getEntries())
		{
			WTDMipmap *pWTDMipmap = new WTDMipmap(pWTDEntry);

			pWTDMipmap->setRasterData(pGeneralMipmap->getRasterData());
			pWTDMipmap->setImageSize(true, pGeneralMipmap->getSize().x);
			pWTDMipmap->setImageSize(false, pGeneralMipmap->getSize().y);

			pWTDEntry->addEntry(pWTDMipmap);
		}

		pWTDFile->addEntry(pWTDEntry);
	}

	return pWTDFile;
}