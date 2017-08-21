#include "CTXDFormat.h"
#include "Static/CString2.h"
#include "CTXDManager.h"
#include "Static/CDebug.h"
#include "Engine/RW/CRWManager.h"
#include "Format/RW/eRWSection.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Static/CPath.h"
#include "Static/CFile.h"
#include "Format/Image/BMP/CBMPManager.h"
#include "Format/Image/BMP/CBMPFormat.h"
#include "Intermediate/Texture/CIntermediateTextureFormat.h"
#include "Intermediate/Texture/CIntermediateTexture.h"
#include "Intermediate/Texture/Data/CIntermediateTextureMipmap.h"
#include "Image/CImageManager.h"
#include "Image/CImageFile.h"
#include "Format/RW/Sections/CRWSection_TextureNative.h"
#include "Type/Vector/Vec2u16.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CTXDFormat::CTXDFormat(void) :
	m_usDeviceId(0)
{
}

vector<CRWSection_TextureNative*>	CTXDFormat::getTextures(void)
{
	vector<CRWSection*> vecRWSections = getSectionsByType(RW_SECTION_TEXTURE_NATIVE);
	vector<CRWSection_TextureNative*> vecTextures;
	vecTextures.resize(vecRWSections.size());
	uint32 i = 0;
	for (CRWSection *pRWSection : vecRWSections)
	{
		vecTextures[i] = (CRWSection_TextureNative*)pRWSection;
		i++;
	}
	return vecTextures;
}

vector<string>		CTXDFormat::getTextureNames(void)
{
	vector<string> vecTextureNames;
	for (auto pTexture : getTextures())
	{
		if (pTexture->doesHaveDiffuse())
		{
			vecTextureNames.push_back(pTexture->getDiffuseName());
		}
		if (pTexture->doesHaveAlpha())
		{
			vecTextureNames.push_back(pTexture->getAlphaName());
		}
	}
	return vecTextureNames;
}

void				CTXDFormat::convertToGame(ePlatformedGame ePlatformedGame, vector<string>& vecMipmapsRemoved2)
{
	m_vecGames.clear();
	m_vecGames.push_back(ePlatformedGame);

	setRWVersion(CRWManager::get()->getVersionManager()->getRWVersionFromGame(ePlatformedGame));
	
	uint32 uiTextureIndex = 0;
	for (auto pTexture : getTextures())
	{
		vector<string> vecMipmapsRemoved;
		pTexture->convertToGame(ePlatformedGame, vecMipmapsRemoved);
		
		if (ePlatformedGame == PLATFORMED_GAME_PC_GTA_SA)
		{
			m_usDeviceId = 2;
		}
		else
		{
			m_usDeviceId = 0;
		}
		
		for (auto strMipmapRemoved : vecMipmapsRemoved)
		{
			strMipmapRemoved = "Texture #" + CString2::toString(uiTextureIndex + 1) + " (" + pTexture->getDiffuseName() + ") - " + strMipmapRemoved;
			vecMipmapsRemoved2.push_back(strMipmapRemoved);
		}

		uiTextureIndex++;
	}

	for (uint32 i = 0, j = vecMipmapsRemoved2.size(); i < j; i++)
	{
		string strMipmapRemoved = vecMipmapsRemoved2[i];
		vecMipmapsRemoved2[i] = "TXD File: " + CPath::getFileName(getFilePath()) + strMipmapRemoved;
	}
}

void				CTXDFormat::convertToRasterDataFormat(eRasterDataFormat eRasterDataFormatValue, vector<string>& vecMipmapsRemoved)
{
	for (auto pTexture : getTextures())
	{
		pTexture->convertToRasterDataFormat(eRasterDataFormatValue, vecMipmapsRemoved);
	}
}

bool				CTXDFormat::doesHaveTextureWithInvalidTXDRasterDataFormat(void)
{
	for (auto pTexture : getTextures())
	{
		if (!pTexture->doesHaveValidTXDRasterDataFormat())
		{
			return true;
		}
	}
	return false;
}

bool				CTXDFormat::isTextureNameValid(string& strTextureName, bool bIsAlphaTexture)
{
	if (bIsAlphaTexture)
	{
		bool bNotBlank = strTextureName.length() > 0 && (strTextureName.c_str()[0] & 0xFF) != 0;
		if (bNotBlank)
		{
			return CString2::isAlphaNumericUnderscore(strTextureName);
		}
		return true;
	}
	else
	{
		bool bNotBlank = strTextureName.length() > 0 && (strTextureName.c_str()[0] & 0xFF) != 0;
		if (bNotBlank)
		{
			return CString2::isAlphaNumericUnderscore(strTextureName);
		}
		return false;
	}
}

bool				CTXDFormat::isTextureResolutionValid(uint16 usWidth, uint16 usHeight, vector<ePlatformedGame>& vecGames)
{
	// ensure width and height are both even
	if ((usWidth % 2) == 1 || (usHeight % 2) == 1)
	{
		// invalid
		return false;
	}

	for (auto ePlatformedGame : vecGames)
	{
		if (ePlatformedGame == PLATFORMED_GAME_PC_GTA_III || ePlatformedGame == PLATFORMED_GAME_PC_GTA_VC)
		{
			// III/VC - 8x8 to 4096x4096
			if (usWidth < 8 || usWidth > 4096 || usHeight < 8 || usHeight > 4096)
			{
				// invalid
			}
			else
			{
				// valid
				return true;
			}
		}
		else if (ePlatformedGame == PLATFORMED_GAME_PC_GTA_SA)
		{
			// SA - 16x16 to 2048x2048
			if (usWidth < 16 || usWidth > 2048 || usHeight < 16 || usHeight > 2048)
			{
				// invalid
			}
			else
			{
				// valid
				return true;
			}
		}
	}

	// invalid
	return false;
}

bool				CTXDFormat::isTXDSizeValid(uint32 uiFileSize)
{
	return uiFileSize <= (200 * 1024 * 1024); // 200 MB
}

bool				CTXDFormat::isTextureCountValid(uint32 uiTextureCount, vector<ePlatformedGame>& vecGames)
{
	for (auto ePlatformedGame : vecGames)
	{
		bool bValid = uiTextureCount <= getMaxTextureCountForGame(ePlatformedGame);
		if (bValid)
		{
			return true;
		}
	}

	return false;
}

uint32		CTXDFormat::getMaxTextureCountForGame(ePlatformedGame ePlatformedGame)
{
	if (ePlatformedGame == PLATFORMED_GAME_PC_GTA_III || ePlatformedGame == PLATFORMED_GAME_PC_GTA_VC)
	{
		return 128;
	}
	else if (ePlatformedGame == PLATFORMED_GAME_PC_GTA_SA)
	{
		return 64;
	}
	return 0xFFFFFFFF;
}

CRWSection_TextureNative*	CTXDFormat::addTextureViaFile(string& strFilePath, string& strTextureDiffuseName, string strTextureAlphaName)
{
	CImageFile *pImageFile = CImageManager::loadImageFromFile(strFilePath);

	CRWSection_TextureNative *pTexture = new CRWSection_TextureNative;
	pTexture->setBPP(32);
	pTexture->setHasDiffuse(true);
	pTexture->setDiffuseName(strTextureDiffuseName);
	if (pImageFile->m_uiBPP == 32 && strTextureAlphaName != "")
	{
		pTexture->setHasAlpha(true);
		pTexture->setAlphaName(strTextureAlphaName);
	}
	else
	{
		string strAlphaName = "";
		
		pTexture->setHasAlpha(false);
		pTexture->setAlphaName(strAlphaName);
	}

	Vec2u vecSize;
	vecSize.x = pImageFile->m_uiImageWidth;
	vecSize.y = pImageFile->m_uiImageHeight;
	string strPaletteData = "";

	pTexture->setDXTCompressionType(DXT_NOT_COMPRESSED);
	pTexture->setFilterFlags(0);
	pTexture->setImageSize(vecSize);
	pTexture->setPaletteUsed(false);
	pTexture->setPaletteData(strPaletteData);
	pTexture->setPlatformId(8);
	pTexture->setTXDRasterDataFormat(TXDRASTERDATAFORMAT_8888);
	pTexture->setRasterDataFormat(RASTERDATAFORMAT_BGRA32, false);
	pTexture->setRasterType(0);
	pTexture->setTextureWrapUV(0);

	CRWEntry_TextureNative_MipMap *pMipmap = new CRWEntry_TextureNative_MipMap(pTexture);
	pMipmap->setImageSize(vecSize);
	pMipmap->setRasterData(pImageFile->m_strRasterDataBGRA32);
	pTexture->getMipMaps().addEntry(pMipmap);

	if (getSectionCountByType(RW_SECTION_GEOMETRY_LIST) > 0)
	{
		getSectionsByType(RW_SECTION_GEOMETRY_LIST)[0]->addEntry(pTexture);
	}

	delete pImageFile;
	return pTexture;
}

bxgi::CIntermediateTextureFormat*	CTXDFormat::convertToIntermediateFormat(void)
{
	bxgi::CIntermediateTextureFormat *pGeneralTextureFile = new bxgi::CIntermediateTextureFormat;

	for (CRWSection_TextureNative *pTXDEntry : getTextures())
	{
		CIntermediateTexture *pGeneralTexture = new CIntermediateTexture;

		if (pTXDEntry->doesHaveDiffuse())
		{
			pGeneralTexture->setName(pTXDEntry->getDiffuseName());
		}
		else if (pTXDEntry->doesHaveAlpha())
		{
			pGeneralTexture->setName(pTXDEntry->getAlphaName());
		}
		else
		{
			string strTextureName = "Unknown";
			pGeneralTexture->setName(strTextureName);
		}

		Vec2u16 vecImageSize;
		vecImageSize.x = (uint16) pTXDEntry->getImageSize().x;
		vecImageSize.y = (uint16) pTXDEntry->getImageSize().y;

		pGeneralTexture->setRasterDataFormat(pTXDEntry->getRasterDataFormat());
		pGeneralTexture->setSize(vecImageSize);

		for (CRWEntry_TextureNative_MipMap *pTXDMipmap : pTXDEntry->getMipMaps().getEntries())
		{
			CIntermediateTextureMipmap *pGeneralMipmap = new CIntermediateTextureMipmap;

			Vec2u16 vecMipmapSize;
			vecMipmapSize.x = (uint16) pTXDMipmap->getImageSize().x;
			vecMipmapSize.y = (uint16) pTXDMipmap->getImageSize().y;

			pGeneralMipmap->setRasterData(pTXDMipmap->getRasterData());
			pGeneralMipmap->setSize(vecMipmapSize);

			pGeneralTexture->addEntry(pGeneralMipmap);
		}

		pGeneralTextureFile->addEntry(pGeneralTexture);
	}

	return pGeneralTextureFile;
}