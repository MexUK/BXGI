#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/Format.h"
#include "Pool/VectorPool.h"
#include "EIMGVersion.h"
#include "Engine/RW/ERWVersion.h"
#include "Compression/ECompressionAlgorithm.h"
#include "Format/IMG/Fastman92/EIMGVersionFastman92CompressionAlgorithm.h"
#include "Format/COL/ECOLVersion.h"
#include "Platform/Hardware/EPlatform.h"
#include "Stream/DataReader.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct SortByStringKey // todo - namespace
{
	bool operator() (const std::string& a, const std::string& b) const
	{
		return strcmp(a.c_str(), b.c_str()) < 0;
	}
};

class bxgi::IMGEntry;

class bxgi::IMGFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::IMGEntry*>
{
public:
	IMGFormat(void);
	IMGFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true);
	IMGFormat(bxcf::DataReader& reader);

	void											unload(void) {}

	void											_readMetaData(void);

	void											_unserialize(void);
	void											_serialize(void);
	bool											validate(void);

	std::string										readEntryContent(uint32 uiEntryIndex);

	std::string										getIMGFilePath(void);
	std::string										getDIRFilePath(void);

	void											setVersion(bxgi::EIMGVersion EIMGVersion) { m_uiIMGVersion = EIMGVersion; }
	inline bxgi::EIMGVersion						getVersion(void);

	void											unserializERWVersions(void);
	void											unserializeResourceTypes(void);

	void											setEncrypted(bool bEncrypted) { m_bEncrypted = bEncrypted; }
	bool											isEncrypted(void) { return m_bEncrypted; }

	void											setGameType(uint8 ucGameType) { m_ucGameType = ucGameType; }
	uint8											getGameType(void) { return m_ucGameType; }

	void											setPlatform(bxcf::EPlatform EPlatformValue) { m_uiPlatform = EPlatformValue; }
	bxcf::EPlatform									getPlatform(void) { return m_uiPlatform; }

	std::vector<bxcf::fileType::EFileType>			getFileTypes(void);
	std::vector<std::string>						getFileTypesText(void);
	std::map<std::string, bxcf::fileType::EFileType, SortByStringKey>	getFileTypesAsMap(void);

	std::vector<std::string>						getFileVersions(void);
	std::map<std::string, std::pair<uint32, bxcf::fileType::EFileType>, SortByStringKey>	getFileTypedVersionsAsMap(void);

	void											getModelAndTextureSetNamesFromEntries(
		std::unordered_map<IMGEntry*, std::vector<std::string>>& umapIMGModelNames,
		std::unordered_map<IMGEntry*, std::vector<std::string>>& umapIMGTextureSetNames
	);






	uint32											getEntryExtensionCount(std::string strEntryExtension);

	static bxcf::ECompressionAlgorithm				getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId(EIMGVersionFastman92CompressionAlgorithm eFastman92CompressionAlgorithmId);

	bxgi::IMGEntry*									addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	bxgi::IMGEntry*									addEntryViaData(std::string& strEntryName, std::string& strEntryData);
	void											addEntry(bxgi::IMGEntry *pIMGEntry);
	void											addEntries(std::vector<std::string>& vecEntryFilePaths);
	void											addAllEntriesInFolder(std::string& strFolderPath);
	void											removeEntry(bxgi::IMGEntry *pIMGEntry);
	void											removeAllEntries(void);
	bxgi::IMGEntry*									replaceEntryViaFile(std::string& strEntryName, std::string& strEntryFilePath, std::string strNewEntryName = "");
	bxgi::IMGEntry*									replaceEntryViaData(std::string& strEntryName, std::string& strEntryData, std::string strNewEntryName = "");
	uint32											replaceEntries(std::vector<std::string>& vecPaths, std::vector<std::string>& vecReplacedEntryNames, std::vector<IMGEntry*>& vecReplacedEntries);
	bxgi::IMGEntry*									addOrReplaceEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	bxgi::IMGEntry*									addOrReplaceEntryViaData(std::string& strEntryName, std::string& strEntryData);
	std::unordered_map<bxgi::IMGEntry*, std::string>	getAllEntriesData(void); // not recommended to be used, may use too much memory, better to iterate pIMGFile->getEntries()
	std::unordered_map<bxgi::IMGEntry*, std::string>	getEntriesData(std::vector<bxgi::IMGEntry*>& vecEntries); // not recommended to be used, may use too much memory, better to iterate pIMGFile->getEntries()
	uint32											getNextEntryOffset(void); // in bytes

	std::vector<bxgi::IMGEntry*>					getEntriesByName(std::string strText); // case-insensitive, wildcard match
	std::vector<bxgi::IMGEntry*>					getEntriesByExtension(std::string strExtension, bool bWildcard = false); // case-insensitive
	std::vector<bxgi::IMGEntry*>					getEntriesByVersion(uint32 uiFileTypeId, uint32 uiFileVersionId);

	bxgi::IMGEntry*									getEntryByName(std::string& strEntryName); // case-insensitive
	bxgi::IMGEntry*									getEntryByNameWithoutExtension(std::string& strEntryNameWithoutExtension); // case-insensitive
	std::vector<std::string>						getEntryNames(void);
	bxgi::IMGEntry*									getEntryByHighestOffset(void);
	uint32											getEntryCountForName(std::string& strEntryName); // case-insensitive
	uint32											getEntryCountForCompressionType(bxcf::ECompressionAlgorithm ECompressionAlgorithmValue);

	static uint32									getEntryPaddedSize(uint32 uiDataLength);
	void											unsetNewAndReplacedFlagForAllEntries(void);

	uint32											merge(std::string& strSecondIMGPath, std::vector<std::string>& vecImportedEntryNames);
	void											split(std::vector<bxgi::IMGEntry*>& vecIMGEntries, std::string& strOutPath, EIMGVersion EIMGVersion);

	void											exportSingle(bxgi::IMGEntry *pIMGEntry, std::string& strFolderPath);
	void											exportMultiple(std::vector<bxgi::IMGEntry*>& vecIMGEntries, std::string strFolderPath);
	void											exportAll(std::string& strFolderPath);

	bool											doesContainEntryWithUnknownRWVersion(void); // for IMG versions 1 and 2 - todo - should it be changed to without RW to match function below - check func bodys
	std::vector<bxgi::IMGEntry*>					getUnknownVersionEntries(void); // for IMG versions 1 and 2

private:
	void											unserializeHeaderComponents(void);
	void											unserializeBodyComponents(void);

	void											serializeHeaderAndBodyComponents(void);
	
	void											unserializeVersion1(void);
	void											unserializeVersion2(void);
	void											unserializeVersion3_Encrypted(void);
	void											unserializeVersion3_Unencrypted(void);
	void											unserializeVersionFastman92(void);

	void											serializeVersion1(void);
	void											serializeVersion2(void);
	void											serializeVersion3_Encrypted(void);
	void											serializeVersion3_Unencrypted(void);
	void											serializeVersionFastman92(void);

	uint32											getVersion3NamesLength(void);

private:
	void											addEntryExtensionCount(std::string strEntryExtension);
	void											removeEntryExtensionCount(std::string strEntryExtension);
	void											loadEntryExtensionCounts(void);

private:
	bxgi::EIMGVersion								m_uiIMGVersion;
	bxcf::EPlatform									m_uiPlatform;
	std::unordered_map<std::string, uint32>			m_umapExtensionCounts;
	uint8											m_uiSubVersion		: 4;
	uint8											m_uiEncryptionType	: 4;
	uint8											m_ucGameType		: 3;
	uint8											m_bEncrypted		: 1;
};