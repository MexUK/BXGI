#include "TXDFormat.h"
#include "Static/String.h"
#include "TXDManager.h"
#include "Static/Debug.h"
#include "Engine/RW/RWManager.h"
#include "Format/RW/ERWSection.h"
#include "Engine/RW/RWVersionManager.h"
#include "Static/Path.h"
#include "Static/File.h"
#include "Format/Image/BMP/BMPManager.h"
#include "Format/Image/BMP/BMPFormat.h"
#include "Intermediate/Texture/IntermediateTextureFormat.h"
#include "Intermediate/Texture/IntermediateTexture.h"
#include "Intermediate/Texture/Data/IntermediateTextureMipmap.h"
#include "Image/ImageManager.h"
#include "Image/ImageFile.h"
#include "Format/RW/Sections/RWSection_TextureNative.h"
#include "Format/RW/Sections/RWSection_TextureDictionary.h"
#include "Format/RW/Sections/RWSection_Extension.h"
#include "Type/Vector/Vec2u16.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Event/Events.h"
#include "../bxcf/Event/EEvent.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

TXDFormat::TXDFormat(void) :
	m_usDeviceId(0)
{
}

void								TXDFormat::_unserialize(void)
{
	RWFormat::_unserialize();
	m_vecTextures = (vector<FormatEntry*>&)getTextures();
}

void								TXDFormat::_serialize(void)
{
	RWFormat::_serialize();
}

RWSection_TextureNative*			TXDFormat::addEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	if (strEntryName == "")
	{
		strEntryName = Path::getFileName(strEntryFilePath);
	}

	return addTextureViaFile(strEntryFilePath, Path::removeFileExtension(strEntryName), Path::removeFileExtension(strEntryName) + "a");
}

RWSection_TextureNative*			TXDFormat::addEntryViaData(string& strEntryName, string& strEntryData)
{
	return addTextureViaData(strEntryData, Path::removeFileExtension(strEntryName), Path::removeFileExtension(strEntryName) + "a");
}

vector<RWSection_TextureNative*>	TXDFormat::getTextures(void)
{
	vector<RWSection*> vecRWSections = getSectionsByType(RW_SECTION_TEXTURE_NATIVE);
	vector<RWSection_TextureNative*> vecTextures;
	vecTextures.resize(vecRWSections.size());
	uint32 i = 0;
	for (RWSection *pRWSection : vecRWSections)
	{
		vecTextures[i] = (RWSection_TextureNative*)pRWSection;
		i++;
	}
	return vecTextures;
}

vector<string>		TXDFormat::getTextureNames(void)
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

void				TXDFormat::convertToGame(EPlatformedGame EPlatformedGame, vector<string>& vecMipmapsRemoved2)
{
	m_vecGames.clear();
	m_vecGames.push_back(EPlatformedGame);

	setRWVersion(RWManager::get()->getVersionManager()->getRWVersionFromGame(EPlatformedGame));
	
	uint32 uiTextureIndex = 0;
	for (auto pTexture : getTextures())
	{
		vector<string> vecMipmapsRemoved;
		pTexture->convertToGame(EPlatformedGame, vecMipmapsRemoved);
		
		if (EPlatformedGame == PC_GTA_SA)
		{
			m_usDeviceId = 2;
		}
		else
		{
			m_usDeviceId = 0;
		}
		
		for (auto strMipmapRemoved : vecMipmapsRemoved)
		{
			strMipmapRemoved = "Texture #" + String::toString(uiTextureIndex + 1) + " (" + pTexture->getDiffuseName() + ") - " + strMipmapRemoved;
			vecMipmapsRemoved2.push_back(strMipmapRemoved);
		}

		uiTextureIndex++;
	}

	for (uint32 i = 0, j = vecMipmapsRemoved2.size(); i < j; i++)
	{
		string strMipmapRemoved = vecMipmapsRemoved2[i];
		vecMipmapsRemoved2[i] = "TXD File: " + Path::getFileName(getFilePath()) + strMipmapRemoved;
	}
}

void				TXDFormat::convertToRasterDataFormat(ERasterDataFormat ERasterDataFormatValue, vector<string>& vecMipmapsRemoved)
{
	for (auto pTexture : getTextures())
	{
		pTexture->convertToRasterDataFormat(ERasterDataFormatValue, vecMipmapsRemoved);
	}
}

bool				TXDFormat::doesHaveTextureWithInvalidTXDRasterDataFormat(void)
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

bool				TXDFormat::isTextureNameValid(string& strTextureName, bool bIsAlphaTexture)
{
	if (bIsAlphaTexture)
	{
		bool bNotBlank = strTextureName.length() > 0 && (strTextureName.c_str()[0] & 0xFF) != 0;
		if (bNotBlank)
		{
			return String::isAlphaNumericUnderscore(strTextureName);
		}
		return true;
	}
	else
	{
		bool bNotBlank = strTextureName.length() > 0 && (strTextureName.c_str()[0] & 0xFF) != 0;
		if (bNotBlank)
		{
			return String::isAlphaNumericUnderscore(strTextureName);
		}
		return false;
	}
}

bool				TXDFormat::isTextureResolutionValid(uint16 usWidth, uint16 usHeight, vector<EPlatformedGame>& vecGames)
{
	// ensure width and height are both even
	if ((usWidth % 2) == 1 || (usHeight % 2) == 1)
	{
		// invalid
		return false;
	}

	for (auto EPlatformedGame : vecGames)
	{
		if (EPlatformedGame == PC_GTA_III || EPlatformedGame == PC_GTA_VC)
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
		else if (EPlatformedGame == PC_GTA_SA)
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

bool				TXDFormat::isTXDSizeValid(uint32 uiFileSize)
{
	return uiFileSize <= (200 * 1024 * 1024); // 200 MB
}

bool				TXDFormat::isTextureCountValid(uint32 uiTextureCount, vector<EPlatformedGame>& vecGames)
{
	for (auto EPlatformedGame : vecGames)
	{
		bool bValid = uiTextureCount <= getMaxTextureCountForGame(EPlatformedGame);
		if (bValid)
		{
			return true;
		}
	}

	return false;
}

uint32		TXDFormat::getMaxTextureCountForGame(EPlatformedGame EPlatformedGame)
{
	if (EPlatformedGame == PC_GTA_III || EPlatformedGame == PC_GTA_VC)
	{
		return 128;
	}
	else if (EPlatformedGame == PC_GTA_SA)
	{
		return 64;
	}
	return 0xFFFFFFFF;
}

RWSection_TextureNative*	TXDFormat::addTextureViaData(string& strFileData, string& strTextureDiffuseName, string strTextureAlphaName)
{
	// todo
	return nullptr;
}

RWSection_TextureNative*	TXDFormat::addTextureViaFile(string& strFilePath, string& strTextureDiffuseName, string strTextureAlphaName)
{
	ImageFile *pImageFile = ImageManager::loadImageFromFile(strFilePath);

	RWSection_TextureNative *pTexture = (RWSection_TextureNative *)RWSection::creatERWSection(RW_SECTION_TEXTURE_NATIVE);
	pTexture->setRWFormat(this);
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
	pTexture->setPlatform(PLATFORM_PC);
	pTexture->setTXDRasterDataFormat(TXDRASTERDATAFORMAT_8888);
	pTexture->setRasterDataFormat(RASTERDATAFORMAT_BGRA32, false);
	pTexture->setRasterType(0);
	pTexture->setTextureWrapUV(0);

	RWEntry_TextureNative_MipMap *pMipmap = new RWEntry_TextureNative_MipMap(pTexture);
	pMipmap->setImageSize(vecSize);
	pMipmap->setRasterData(pImageFile->m_strRasterDataBGRA32);
	pTexture->getMipMaps().addEntry(pMipmap);

	if (getSectionCountByType(RW_SECTION_TEXTURE_DICTIONARY) > 0)
	{
		if (getSectionsByType(RW_SECTION_TEXTURE_DICTIONARY)[0]->getEntryCount() > 0 && getSectionsByType(RW_SECTION_TEXTURE_DICTIONARY)[0]->getLastEntry()->getSectionId() == RW_SECTION_EXTENSION)
		{
			getSectionsByType(RW_SECTION_TEXTURE_DICTIONARY)[0]->addEntryAtPosition(pTexture, getSectionsByType(RW_SECTION_TEXTURE_DICTIONARY)[0]->getEntryCount() - 1);
		}
		else
		{
			getSectionsByType(RW_SECTION_TEXTURE_DICTIONARY)[0]->addEntry(pTexture);
		}
	}

	delete pImageFile;
	return pTexture;
}

IntermediateTextureFormat*	TXDFormat::convertToIntermediateFormat(void)
{
	IntermediateTextureFormat *pGeneralTextureFile = new IntermediateTextureFormat;

	for (RWSection_TextureNative *pTXDEntry : getTextures())
	{
		IntermediateTexture *pGeneralTexture = new IntermediateTexture;

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

		for (RWEntry_TextureNative_MipMap *pTXDMipmap : pTXDEntry->getMipMaps().getEntries())
		{
			IntermediateTextureMipmap *pGeneralMipmap = new IntermediateTextureMipmap;

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

void						TXDFormat::exportSingle(FormatEntry *pEntry, string& strFolderPath)
{
	string strFolderPath2 = Path::addSlashToEnd(strFolderPath);
	File::setBinaryFile(strFolderPath2 + pEntry->getEntryName(), readEntryContent(pEntry->getIndex()));
}

void						TXDFormat::exportMultiple(vector<FormatEntry*>& vecEntries, string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getFilePath());
	if (!openFile())
	{
		return;
	}

	for (RWSection_TextureNative *pTXDEntry : (vector<RWSection_TextureNative*>&)vecEntries)
	{
		RWEntry_TextureNative_MipMap *pMipmap = pTXDEntry->getMipMaps().getEntryByIndex(0);

		BMPFormat *pBMPFile = new BMPFormat;
		pBMPFile->setWidth(pMipmap->getImageSize().x);
		pBMPFile->setHeight(pMipmap->getImageSize().y);
		pBMPFile->setBPP(32);

		pBMPFile->setRasterData(pMipmap->getRasterDataBGRA32());
		pBMPFile->swapRows();

		string strBMPFilePath = strFolderPath + "/" + pTXDEntry->getDiffuseName() + ".BMP";
		strBMPFilePath = Path::getNextFileName(strBMPFilePath, 1, "-Mipmap");

		pBMPFile->setBMPVersion(3);
		pBMPFile->serialize(strBMPFilePath);

		pBMPFile->unload();
		delete pBMPFile;

		Events::trigger(TASK_PROGRESS);
	}

	m_reader.close();
}

void						TXDFormat::exportAll(string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getFilePath());
	if (!openFile())
	{
		return;
	}

	for (RWSection_TextureNative *pTXDEntry : (vector<RWSection_TextureNative*>&)getSectionsByType(RW_SECTION_TEXTURE_NATIVE))
	{
		RWEntry_TextureNative_MipMap *pMipmap = pTXDEntry->getMipMaps().getEntryByIndex(0);

		BMPFormat *pBMPFile = new BMPFormat;
		pBMPFile->setWidth(pMipmap->getImageSize().x);
		pBMPFile->setHeight(pMipmap->getImageSize().y);
		pBMPFile->setBPP(32);

		pBMPFile->setRasterData(pMipmap->getRasterDataBGRA32());
		pBMPFile->swapRows();

		string strBMPFilePath = strFolderPath + "/" + pTXDEntry->getDiffuseName() + ".BMP";
		strBMPFilePath = Path::getNextFileName(strBMPFilePath, 1, "-Mipmap");

		pBMPFile->setBMPVersion(3);
		pBMPFile->serialize(strBMPFilePath);

		pBMPFile->unload();
		delete pBMPFile;

		Events::trigger(TASK_PROGRESS);
	}

	m_reader.close();
}

RWSection_TextureNative*	TXDFormat::getEntryByName(string& strEntryName)
{
	string
		strEntryName2 = String::toUpperCase(strEntryName),
		strEntryName2NoExt = String::toUpperCase(Path::removeFileExtension(strEntryName));
	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_TEXTURE_NATIVE))
	{
		RWSection_TextureNative *pTextureNative = (RWSection_TextureNative *)pRWSection;
		if (strEntryName2 == String::toUpperCase(pTextureNative->getDiffuseName()) || strEntryName2 == String::toUpperCase(pTextureNative->getAlphaName()))
		{
			return pTextureNative;
		}
		if (strEntryName2NoExt == String::toUpperCase(Path::removeFileExtension(pTextureNative->getDiffuseName())) || strEntryName2NoExt == Path::removeFileExtension(String::toUpperCase(pTextureNative->getAlphaName())))
		{
			return pTextureNative;
		}
	}
	return nullptr;
}

vector<FormatEntry*>		TXDFormat::getAllEntries(void)
{
	vector<FormatEntry*> vecEntries;
	for (RWSection *pRWSection: getSectionsByType(RW_SECTION_TEXTURE_NATIVE))
	{
		vecEntries.push_back((RWSection_TextureNative*)pRWSection);
	}
	return vecEntries;
}

void						TXDFormat::swapEntries(FormatEntry *pEntry1, FormatEntry *pEntry2)
{
	RWSection_TextureDictionary *pTextureDictionary = (RWSection_TextureDictionary*)getSectionsByType(RW_SECTION_TEXTURE_DICTIONARY)[0];
	pTextureDictionary->swapEntries((RWSection_TextureNative*)pEntry1, (RWSection_TextureNative*)pEntry2);
}

void						TXDFormat::merge(string& strFilePath)
{
	DataReader *pDataReader = &m_reader;

	// parse second file for entry information
	TXDFormat *pFileIn = TXDManager::unserializeFile(strFilePath);
	pFileIn->setFilePath(strFilePath);
	if (!pFileIn->unserialize())
	{
		pFileIn->unload();
		delete pFileIn;
		return;
	}

	// open second file to read entry body data from
	pDataReader->setFilePath(getFilePath());
	pDataReader->open(true);

	// import entries from second file into first file
	for (RWSection *pRWSection : pFileIn->getSectionsByType(RW_SECTION_TEXTURE_NATIVE))
	{
		RWSection_TextureNative *pInEntry = (RWSection_TextureNative *)pRWSection;

		// entry data
		RWSection_TextureNative *pOutEntry = new RWSection_TextureNative;
		pOutEntry->set2dEffects(pInEntry->get2dEffects());
		pOutEntry->setAlpha(pInEntry->getAlpha());
		pOutEntry->setAlphaName(pInEntry->getAlphaName());
		pOutEntry->setAutoMipMaps(pInEntry->getAutoMipMaps());
		pOutEntry->setBPP(pInEntry->getBPP());
		pOutEntry->setCubeTexture(pInEntry->getCubeTexture());
		pOutEntry->setDiffuseName(pInEntry->getDiffuseName());
		pOutEntry->setDVColours(pInEntry->getDVColours());
		pOutEntry->setDXTCompressionType(pInEntry->getDXTCompressionType());
		pOutEntry->setFilterFlags(pInEntry->getFilterFlags());
		pOutEntry->setHasAlpha(pInEntry->doesHaveAlpha());
		pOutEntry->setHasDiffuse(pInEntry->doesHaveDiffuse());
		pOutEntry->setImageSize(pInEntry->getImageSize());
		pOutEntry->setIsNotRWCompatible(pInEntry->getIsNotRWCompatible());
		pOutEntry->setMipMapCount(pInEntry->getMipMapCount());
		pOutEntry->setNVColours(pInEntry->getNVColours());
		pOutEntry->setOriginalBPP(pInEntry->getOriginalBPP());
		pOutEntry->setPaletteData(pInEntry->getPaletteData());
		pOutEntry->setPaletteUsed(pInEntry->isPaletteUsed());
		pOutEntry->setPlatform(pInEntry->getPlatform());
		pOutEntry->setPlatformId(pInEntry->getPlatformId());
		pOutEntry->setRasterDataFormat(pInEntry->getRasterDataFormat());
		pOutEntry->setRasterType(pInEntry->getRasterType());
		pOutEntry->setRWFormat(pInEntry->getRWFormat());
		pOutEntry->setSectionHeaderSkipped(pInEntry->isSectionHeaderSkipped());
		pOutEntry->setSectionId(pInEntry->getSectionId());
		pOutEntry->setSectionRWVersion(pInEntry->getSectionRWVersion());
		pOutEntry->setSectionSize(pInEntry->getSectionSize());
		pOutEntry->setStructSectionSize(pInEntry->getStructSectionSize());
		pOutEntry->setTextureWrapUV(pInEntry->getTextureWrapUV());
		pOutEntry->setTXDRasterDataFormat(pInEntry->getTXDRasterDataFormat());
		pOutEntry->setUnknownSection(pInEntry->isUnknownSection());

		for (uint32 i = 0, j = pInEntry->getMipMapCount(); i < j; i++)
		{
			RWEntry_TextureNative_MipMap *pMipMapIn = pInEntry->getMipMaps().getEntryByIndex(i);
			RWEntry_TextureNative_MipMap *pMipMapOut = new RWEntry_TextureNative_MipMap(pOutEntry);

			pMipMapOut->setImageSize(pMipMapIn->getImageSize());
			pMipMapOut->setRasterData(pMipMapIn->getRasterData());
			pMipMapOut->setSwizzledImageSize(pMipMapIn->getSwizzledImageSize());

			pOutEntry->getMipMaps().addEntry(pMipMapOut);
		}

		addEntry(pOutEntry);
		m_uiEntryCount++;

		Events::trigger(TASK_PROGRESS);
	}

	// finalize
	pDataReader->close();
	pFileIn->unload();
}

// split
void						TXDFormat::split(vector<FormatEntry*>& vecEntries, string& strOutPath, uint32 uiVersion)
{
	TXDFormat *pNewFile = new TXDFormat(strOutPath, true);

	pNewFile->setFilePath(getFilePath()); // todo - rename to format::setInputPath()
	pNewFile->m_writer.setFilePath(strOutPath);

	// todo - remove from here? pIMGFile->openFile(); // open input - todo - rename to openInputFile()

	RWSection_TextureDictionary *pTextureDictionary = new RWSection_TextureDictionary;
	pTextureDictionary->setRWFormat(pNewFile);
	pNewFile->addEntry(pTextureDictionary);

	for (FormatEntry *pEntry : vecEntries)
	{
		RWSection_TextureNative *pInEntry = (RWSection_TextureNative *)pEntry;

		// entry data
		RWSection_TextureNative *pOutEntry = new RWSection_TextureNative;
		pOutEntry->setRWFormat(pNewFile);
		pOutEntry->set2dEffects(pInEntry->get2dEffects());
		pOutEntry->setAlpha(pInEntry->getAlpha());
		pOutEntry->setAlphaName(pInEntry->getAlphaName());
		pOutEntry->setAutoMipMaps(pInEntry->getAutoMipMaps());
		pOutEntry->setBPP(pInEntry->getBPP());
		pOutEntry->setCubeTexture(pInEntry->getCubeTexture());
		pOutEntry->setDiffuseName(pInEntry->getDiffuseName());
		pOutEntry->setDVColours(pInEntry->getDVColours());
		pOutEntry->setDXTCompressionType(pInEntry->getDXTCompressionType());
		pOutEntry->setFilterFlags(pInEntry->getFilterFlags());
		pOutEntry->setHasAlpha(pInEntry->doesHaveAlpha());
		pOutEntry->setHasDiffuse(pInEntry->doesHaveDiffuse());
		pOutEntry->setImageSize(pInEntry->getImageSize());
		pOutEntry->setIsNotRWCompatible(pInEntry->getIsNotRWCompatible());
		pOutEntry->setMipMapCount(pInEntry->getMipMapCount());
		pOutEntry->setNVColours(pInEntry->getNVColours());
		pOutEntry->setOriginalBPP(pInEntry->getOriginalBPP());
		pOutEntry->setPaletteData(pInEntry->getPaletteData());
		pOutEntry->setPaletteUsed(pInEntry->isPaletteUsed());
		pOutEntry->setPlatform(pInEntry->getPlatform());
		pOutEntry->setPlatformId(pInEntry->getPlatformId());
		pOutEntry->setRasterDataFormat(pInEntry->getRasterDataFormat());
		pOutEntry->setRasterType(pInEntry->getRasterType());
		pOutEntry->setRWFormat(pInEntry->getRWFormat());
		pOutEntry->setSectionHeaderSkipped(pInEntry->isSectionHeaderSkipped());
		pOutEntry->setSectionId(pInEntry->getSectionId());
		pOutEntry->setSectionRWVersion(pInEntry->getSectionRWVersion());
		pOutEntry->setSectionSize(pInEntry->getSectionSize());
		pOutEntry->setStructSectionSize(pInEntry->getStructSectionSize());
		pOutEntry->setTextureWrapUV(pInEntry->getTextureWrapUV());
		pOutEntry->setTXDRasterDataFormat(pInEntry->getTXDRasterDataFormat());
		pOutEntry->setUnknownSection(pInEntry->isUnknownSection());

		for (uint32 i = 0, j = pInEntry->getMipMapCount(); i < j; i++)
		{
			RWEntry_TextureNative_MipMap *pMipMapIn = pInEntry->getMipMaps().getEntryByIndex(i);
			RWEntry_TextureNative_MipMap *pMipMapOut = new RWEntry_TextureNative_MipMap(pOutEntry);

			pMipMapOut->setImageSize(pMipMapIn->getImageSize());
			pMipMapOut->setRasterData(pMipMapIn->getRasterData());
			pMipMapOut->setSwizzledImageSize(pMipMapIn->getSwizzledImageSize());

			pOutEntry->getMipMaps().addEntry(pMipMapOut);
		}

		pTextureDictionary->addEntry(pOutEntry);
		m_uiEntryCount++;

		Events::trigger(TASK_PROGRESS);
	}

	RWSection_Extension *pExtension = new RWSection_Extension;
	pExtension->setRWFormat(pNewFile);
	pTextureDictionary->addEntry(pExtension);

	pNewFile->serialize(strOutPath);

	pNewFile->closeOutput(); // todo - still needed?

	pNewFile->unload();
	delete pNewFile;
}