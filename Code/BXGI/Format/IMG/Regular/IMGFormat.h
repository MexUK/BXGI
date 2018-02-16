#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/ContainerFormat.h"
#include "Pool/VectorPool.h"
#include "EIMGVersion.h"
#include "Engine/RW/ERWVersion.h"
#include "Compression/ECompressionAlgorithm.h"
#include "Format/IMG/Fastman92/EIMGVersionFastman92CompressionAlgorithm.h"
#include "Format/COL/ECOLVersion.h"
#include "Platform/Hardware/EPlatform.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IMG/Regular/IMGEntry.h"
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

class bxgi::IMGFormat : public bxcf::ContainerFormat, public bxcf::VectorPool<bxgi::IMGEntry*>
{
public:
	using bxcf::VectorPool<bxgi::IMGEntry*>::getEntryCount;
	using bxcf::VectorPool<bxgi::IMGEntry*>::getEntries;

	IMGFormat(void);
	IMGFormat(EIMGVersion uiIMGVersion);
	IMGFormat(EIMGVersion uiIMGVersion, std::string& strFilePathOrData, bool bStringIsFilePath = true);
	IMGFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true);
	IMGFormat(bxcf::DataReader& reader);

	void											unload(void) {}

	void											_readMetaData(void);

	void											_unserialize(void) {}
	void											_serialize(void) {}

	void											_unserializeBefore(void);
	void											_unserializeAfter(void);

	bool											validate(void);

	static IMGFormat*								createIMGFormat(EIMGVersion uiIMGVersion);
	static IMGFormat*								createIMGFormat(EIMGVersion uiIMGVersion, std::string& strIMGFilePath, bool bParam1IsFilePath = true);
	static IMGFormat*								createIMGFormat(std::string& strIMGFilePath, bool bParam1IsFilePath = true);

	std::string										readEntryContent(uint32 uiEntryIndex);

	void											exportSingle(bxcf::FormatEntry *pEntry, std::string& strFolderPath);
	void											exportMultiple(std::vector<bxcf::FormatEntry*>& vecIMGEntries, std::string& strFolderPath);
	void											exportMultiple(std::vector<bxgi::IMGEntry*>& vecIMGEntries, std::string& strFolderPath);
	void											exportAll(std::string& strFolderPath);

	bxgi::IMGEntry*									getEntryByName(std::string& strEntryName); // case-insensitive

	void											swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2);

	std::vector<bxcf::FormatEntry*>					getAllEntries(void);
	std::vector<bxcf::FormatEntry*>&				getEntriesRef(void) { return (std::vector<bxcf::FormatEntry*>&)m_vecEntries; }

	uint32											getRawVersion(void);

	void											merge(std::string& strFilePath);
	void											split(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFilePathOut, uint32 uiFileVersionOut);





	std::string										getIMGFilePath(void);
	std::string										getDIRFilePath(void);

	void											setVersionDirect(bxgi::EIMGVersion uiIMGVersion) { m_uiIMGVersion = uiIMGVersion; }
	IMGFormat*										setVersion(bxgi::EIMGVersion uiIMGVersion);
	inline bxgi::EIMGVersion						getVersion(void);

	void											unserializERWVersions(void);
	void											unserializeResourceTypes(void);

	void											setEncrypted(bool bEncrypted) { m_bEncrypted = bEncrypted; }
	bool											isEncrypted(void) { return m_bEncrypted; }

	void											setGameType(uint8 ucGameType) { m_ucGameType = ucGameType; }
	uint8											getGameType(void) { return m_ucGameType; }

	void											setSubVersion(uint8 uiSubVersion) { m_uiSubVersion = uiSubVersion; }
	uint8											getSubVersion(void) { return m_uiSubVersion; }

	void											setEncryptionType(uint8 uiEncryptionType) { m_uiEncryptionType = uiEncryptionType; }
	uint8											getEncryptionType(void) { return m_uiEncryptionType; }

	void											setPlatform(bxcf::EPlatform EPlatformValue) { m_uiPlatform = EPlatformValue; }
	bxcf::EPlatform									getPlatform(void) { return m_uiPlatform; }

	std::vector<bxcf::fileType::EFileType>			getFileTypes(void);
	std::vector<std::string>						getFileTypesText(void);
	std::map<std::string, bxcf::fileType::EFileType, SortByStringKey>	getFileTypesAsMap(bool bIncludePercentages = false);

	std::vector<std::string>						getFileVersions(void);
	std::map<std::string, std::pair<uint32, bxcf::fileType::EFileType>, SortByStringKey>	getFileTypedVersionsAsMap(bool bIncludePercentages = false);

	void											getModelAndTextureSetNamesFromEntries(
		std::unordered_map<IMGEntry*, std::vector<std::string>>& umapIMGModelNames,
		std::unordered_map<IMGEntry*, std::vector<std::string>>& umapIMGTextureSetNames
	);






	uint32											getEntryExtensionCount(std::string strEntryExtension);

	static bxcf::ECompressionAlgorithm				getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId(EIMGVersionFastman92CompressionAlgorithm eFastman92CompressionAlgorithmId);

	virtual bxgi::IMGEntry*							addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	virtual bxgi::IMGEntry*							addEntryViaData(std::string& strEntryName, std::string& strEntryData);
	bxgi::IMGEntry*									replaceEntryViaFile(std::string& strEntryFilePath, std::string strExistingEntryName = "", std::string strNewEntryName = "");
	bxgi::IMGEntry*									replaceEntryViaData(std::string& strEntryData, std::string strExistingEntryName, std::string strNewEntryName = "");
	void											addEntry(bxgi::IMGEntry *pIMGEntry);
	void											addEntries(std::vector<std::string>& vecEntryFilePaths);
	void											addAllEntriesInFolder(std::string& strFolderPath);
	void											removeEntry(bxgi::IMGEntry *pIMGEntry);
	void											removeAllEntries(void);
	uint32											replaceEntries(std::vector<std::string>& vecPaths, std::vector<std::string>& vecReplacedEntryNames, std::vector<IMGEntry*>& vecReplacedEntries);
	bxgi::IMGEntry*									addOrReplaceEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	bxgi::IMGEntry*									addOrReplaceEntryViaData(std::string& strEntryName, std::string& strEntryData);
	std::unordered_map<bxgi::IMGEntry*, std::string>	getAllEntriesData(void); // not recommended to be used, may use too much memory, better to iterate pIMGFile->getEntries()
	std::unordered_map<bxgi::IMGEntry*, std::string>	getEntriesData(std::vector<bxgi::IMGEntry*>& vecEntries); // not recommended to be used, may use too much memory, better to iterate pIMGFile->getEntries()
	uint32											getNextEntryOffset(void); // in bytes

	bxgi::IMGEntry*									getEntryByNameWithoutExtension(std::string& strEntryNameWithoutExtension); // case-insensitive
	std::vector<std::string>						getEntryNames(void);
	bxgi::IMGEntry*									getEntryByHighestOffset(void);
	uint32											getEntryCountForName(std::string& strEntryName, bool bWilcard = false, bool bWildcardIsLeftMatch = false); // case-insensitive
	uint32											getEntryCountForCompressionType(bxcf::ECompressionAlgorithm ECompressionAlgorithmValue);

	static uint32									getEntryPaddedSize(uint32 uiDataLength);
	void											unsetNewAndReplacedFlagForAllEntries(void);

	bool											doesContainEntryWithUnknownRWVersion(void); // for IMG versions 1 and 2 - todo - should it be changed to without RW to match function below - check func bodys
	std::vector<bxgi::IMGEntry*>					getUnknownVersionEntries(void); // for IMG versions 1 and 2

protected:
	uint32											getVersion3NamesLength(void);

private:
	void											addEntryExtensionCount(std::string strEntryExtension);
	void											removeEntryExtensionCount(std::string strEntryExtension);
	void											loadEntryExtensionCounts(void);

	void											unserializeAppDataComponents(void);

	void											checkXBOXCompressionStatusesForEntries(void);

private:
	bxgi::EIMGVersion								m_uiIMGVersion;
	bxcf::EPlatform									m_uiPlatform;
	std::unordered_map<std::string, uint32>			m_umapExtensionCounts;
	uint8											m_uiSubVersion		: 4;
	uint8											m_uiEncryptionType	: 4;
	uint8											m_ucGameType		: 3;
	uint8											m_bEncrypted		: 1;
};