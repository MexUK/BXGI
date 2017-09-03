#include <Windows.h>
//#include <d3d11_1.h>
#include <d2d1.h>
#include <DXGIFormat.h>
#include "TXDManager.h"
#include "TXDFormat.h"
#include "Format/RW/Sections/RWSection_TextureNative.h"
#include "Stream/DataReader.h"
#include "Static/String.h"
#include "Engine/RW/RWManager.h"
#include "Static/Debug.h"
#include "Image/RasterDataFormatManager.h"
#include "Engine/RW/RWVersionManager.h"
#include "Intermediate/Texture/IntermediateTextureFormat.h"
#include "Intermediate/Texture/IntermediateTexture.h"
#include "Intermediate/Texture/Data/IntermediateTextureMipmap.h"
#include "Image/ImageManager.h"
#include "Localization/LocalizationManager.h"
#include "Type/Vector/Vec2u16.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

TXDManager::TXDManager(void)
{
}

void					TXDManager::init(void)
{
}
void					TXDManager::uninit(void)
{
}

TXDFormat*				TXDManager::createFormat(void)
{
	TXDFormat *pTXDFile = new TXDFormat;
	pTXDFile->setDeviceId(0);

	RWSection *pTextureDictionary = pTXDFile->addSection(RW_SECTION_TEXTURE_DICTIONARY, RW_3_4_0_3); // todo - make RW version not fixed
	RWSection *pTextureNative = pTextureDictionary->addSection(RW_SECTION_TEXTURE_NATIVE, RW_3_4_0_3); // todo - make RW version not fixed
	RWSection *pExtension1 = pTextureDictionary->addSection(RW_SECTION_EXTENSION, RW_3_4_0_3); // todo - make RW version not fixed
	RWSection *pExtension2 = pTXDFile->addSection(RW_SECTION_EXTENSION, RW_3_4_0_3); // todo - make RW version not fixed

	return pTXDFile;
}

TXDFormat*				TXDManager::convertIntermediateTextureFileToTXDFile(IntermediateTextureFormat *pGeneralTextureFile)
{
	TXDFormat *pTXDFile = createFormat();
	pTXDFile->setRWVersion(RWManager::get()->getVersionManager()->getRWVersionFromGame(PC_GTA_SA));

	for (IntermediateTexture *pGeneralTexture : pGeneralTextureFile->getEntries())
	{
		RWSection_TextureNative *pTexture = new RWSection_TextureNative;

		string strAlphaName = "";

		// struct TextureFormat
		pTexture->setPlatformId(9);
		pTexture->setFilterFlags(0);
		pTexture->setTextureWrapUV(0);
		pTexture->setHasDiffuse(true);
		pTexture->setDiffuseName(pGeneralTexture->getName());
		pTexture->setHasAlpha(false);
		pTexture->setAlphaName(strAlphaName);

		// struct RasterFormat
		ERasterDataFormat ERasterDataFormatValue = RASTERDATAFORMAT_BGRA32;
		pTexture->setRasterDataFormat(ERasterDataFormatValue);
		pTexture->setTXDRasterDataFormat(TXDManager::getTXDRasterDataFormatFromRasterDataFormat(ERasterDataFormatValue));
		EDXTCompressionType eDXTValue = ImageManager::getDXTCompressionTypeFromRasterDataFormat(ERasterDataFormatValue);
		if (pTexture->getPlatformId() == 9)
		{
			// GTA SA
			pTexture->setDXTCompressionType(eDXTValue);
			pTexture->setAlpha(0);
		}
		else
		{
			// GTA III & VC
			pTexture->setAlpha(0);
		}
		Vec2u vecImageSize;
		vecImageSize.x = pGeneralTexture->getSize().x;
		vecImageSize.y = pGeneralTexture->getSize().y;
		pTexture->setImageSize(vecImageSize);
		pTexture->setBPP(ImageManager::getBPPFromRasterDataFormat(ERasterDataFormatValue));
		pTexture->setRasterType(0);
		if (pTexture->getPlatformId() == 9)
		{
			// GTA SA
			uint8 ucSAData = 0;
			pTexture->setAlpha((ucSAData & 1) == 1);
			pTexture->setCubeTexture((ucSAData & 2) == 2);
			pTexture->setAutoMipMaps((ucSAData & 4) == 4);
			pTexture->setIsNotRWCompatible((ucSAData & 8) == 8);
		}
		else
		{
			// GTA III & VC
			pTexture->setDXTCompressionType(eDXTValue);
			pTexture->setCubeTexture(false);
			pTexture->setAutoMipMaps(false);
			pTexture->setIsNotRWCompatible(false);
		}

		for (IntermediateTextureMipmap *pGeneralMipmap : pGeneralTexture->getEntries())
		{
			RWEntry_TextureNative_MipMap *pMipmap = new RWEntry_TextureNative_MipMap(pTexture);

			string strMipmapRasterData = pGeneralMipmap->getRasterData();
			strMipmapRasterData = ImageManager::convertRasterDataFormatToBGRA32(strMipmapRasterData, pGeneralTexture->getRasterDataFormat(), pGeneralTexture->getPaletteData(), pGeneralMipmap->getSize().x, pGeneralMipmap->getSize().y);
			string strPaletteDataOut = "";
			strMipmapRasterData = ImageManager::convertBGRA32ToRasterDataFormat(strMipmapRasterData, ERasterDataFormatValue, &strPaletteDataOut, pGeneralMipmap->getSize().x, pGeneralMipmap->getSize().y);
			
			Vec2u vecMipMapSize;
			vecMipMapSize.x = pGeneralMipmap->getSize().x;
			vecMipMapSize.y = pGeneralMipmap->getSize().y;

			pMipmap->setImageSize(vecMipMapSize);
			pMipmap->setRasterData(strMipmapRasterData);

			pTexture->getMipMaps().addEntry(pMipmap);
		}

		pTXDFile->getTextures().push_back(pTexture);
	}

	return pTXDFile;
}

uint32					TXDManager::getTXDRasterDataFormatFromRasterDataFormat(ERasterDataFormat ERasterDataFormatValue)
{
	switch (ERasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:
	case RASTERDATAFORMAT_DXT2:
	case RASTERDATAFORMAT_DXT3:
	case RASTERDATAFORMAT_DXT4:
	case RASTERDATAFORMAT_DXT5:
		return TXDRASTERDATAFORMAT_565;
	case RASTERDATAFORMAT_BGRA32:
	case RASTERDATAFORMAT_RGBA32:
		return TXDRASTERDATAFORMAT_8888;
	case RASTERDATAFORMAT_BGR32:
	case RASTERDATAFORMAT_RGB32:
	case RASTERDATAFORMAT_BGR24:
	case RASTERDATAFORMAT_RGB24:
		return TXDRASTERDATAFORMAT_888;
	case RASTERDATAFORMAT_PAL8:
		return TXDRASTERDATAFORMAT_8888 | TXDRASTERDATAFORMAT_EXT_PAL4;
	case RASTERDATAFORMAT_PAL4:
		return TXDRASTERDATAFORMAT_8888 | TXDRASTERDATAFORMAT_EXT_PAL8;
	}
	return TXDRASTERDATAFORMAT_DEFAULT;
}

string					TXDManager::getTXDRasterFormatText(uint32 uiTXDRasterDataFormatValue, EDXTCompressionType EDXTCompressionTypeValue)
{
	switch (uiTXDRasterDataFormatValue & 0xFFB)
	{
	case TXDRASTERDATAFORMAT_8888:	return "8888";
	case TXDRASTERDATAFORMAT_888:	return "888";
	case TXDRASTERDATAFORMAT_565:
	case TXDRASTERDATAFORMAT_1555:
	case TXDRASTERDATAFORMAT_555:
	case TXDRASTERDATAFORMAT_4444:
		switch (EDXTCompressionTypeValue)
		{
		case DXT_1:	return "DXT1";
		case DXT_2:	return "DXT2";
		case DXT_3:	return "DXT3";
		case DXT_4:	return "DXT4";
		case DXT_5:	return "DXT5";
		}
	}
	return LocalizationManager::get()->getTranslatedText("UnknownTXDRasterDataFormat");
}

bxcf::EPlatform				TXDManager::getPlatformFromTXDPlatformId(uint32 uiTXDPlatformId)
{
	switch (uiTXDPlatformId)
	{
	case 8: // PC - GTA III & GTA VC
	case 9: // PC - GTA SA
		return PLATFORM_PC;
	case 5: // Xbox - GTA III, GTA VC & GTA SA
		return PLATFORM_XBOX;
	case 12:
		return PLATFORM_ANDROID;
	case 0x00325350:
		return PLATFORM_PS2;
	}
	return _PLATFORM_UNKNOWN;
}