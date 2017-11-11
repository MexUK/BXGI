#pragma once

#include "nsbxgi.h"
#include "Format/RW/RWFormat.h"
#include "Format/RW/RWSection.h"
#include "Game/EPlatformedGame.h"
#include "Image/ERasterDataFormat.h"
#include "Engine/RW/ERWVersion.h"
#include "Format/RW/Sections/RWSection_TextureNative.h"
#include <string>
#include <vector>

class bxgi::RWSection_TextureNative;
class bxgi::IntermediateTextureFormat;

class bxgi::TXDFormat : public bxgi::RWFormat
{
public:
	TXDFormat(void);
	TXDFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxgi::RWFormat(strFilePathOrData, bStringIsFilePath) {}

	virtual bxgi::RWSection_TextureNative*			addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	virtual bxgi::RWSection_TextureNative*			addEntryViaData(std::string& strEntryName, std::string& strEntryData);

	bxgi::RWSection_TextureNative*					addTextureViaData(std::string& strFileData, std::string& strTextureDiffuseName, std::string strTextureAlphaName = "");
	bxgi::RWSection_TextureNative*					addTextureViaFile(std::string& strFilePath, std::string& strTextureDiffuseName, std::string strTextureAlphaName = "");
	
	void											exportSingle(bxcf::FormatEntry *pEntry, std::string& strFolderPath);
	void											exportMultiple(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFolderPath);
	void											exportAll(std::string& strFolderPath);
	
	bxgi::RWSection_TextureNative*					getEntryByName(std::string& strEntryName); // case-insensitive

	std::vector<bxcf::FormatEntry*>					getAllEntries(void);
	std::vector<bxcf::FormatEntry*>&				getEntriesRef(void) { return (std::vector<bxcf::FormatEntry*>&)m_vecTextures; }

	void											swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2);

	void											merge(std::string& strFilePath);
	void											split(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFilePathOut, uint32 uiFileVersionOut);



	std::vector<std::string>						getTextureNames(void);
	std::vector<bxgi::RWSection_TextureNative*>		getTextures(void);

	bool									doesHaveTextureWithInvalidTXDRasterDataFormat(void);

	void									convertToGame(bxgi::EPlatformedGame EPlatformedGame, std::vector<std::string>& vecMipmapsRemoved);
	void									convertToRasterDataFormat(bxcf::ERasterDataFormat ERasterDataFormatValue, std::vector<std::string>& vecMipmapsRemoved);
	bxgi::IntermediateTextureFormat*		convertToIntermediateFormat(void);

	static bool								isTextureNameValid(std::string& strTextureName, bool bIsAlphaTexture = false);
	static bool								isTextureResolutionValid(uint16 usWidth, uint16 usHeight, std::vector<bxgi::EPlatformedGame>& vecGames);
	static bool								isTXDSizeValid(uint32 uiFileSize);
	static bool								isTextureCountValid(uint32 uiTextureCount, std::vector<bxgi::EPlatformedGame>& vecGames);
	static uint32							getMaxTextureCountForGame(bxgi::EPlatformedGame EPlatformedGame);

	void									setPlatformedGames(std::vector<bxgi::EPlatformedGame>& vecGames) { m_vecGames = vecGames; }
	std::vector<bxgi::EPlatformedGame>&		getPlatformedGames(void) { return m_vecGames; }

	void									setDeviceId(uint16 usDeviceId) { m_usDeviceId = usDeviceId; }
	uint16									getDeviceId(void) { return m_usDeviceId; }

private:
	void									_unserialize(void);
	void									_serialize(void);

private:
	std::vector<bxgi::EPlatformedGame>		m_vecGames;
	uint16									m_usDeviceId;
	std::vector<bxcf::FormatEntry*>			m_vecTextures;
};