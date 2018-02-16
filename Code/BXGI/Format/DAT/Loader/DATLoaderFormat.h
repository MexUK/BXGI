#pragma once

#include "nsbxgi.h"
#include "Format/ContainerFormat.h"
#include "Pool/VectorPool.h"
#include "EDATLoaderEntryType.h"
#include "DATLoaderEntry.h"
#include <string>
#include <vector>

class bxgi::IMGFormat;
class bxgi::IDEFormat;
class bxgi::IPLFormat;

class bxgi::DATLoaderFormat : public bxcf::ContainerFormat, public bxcf::VectorPool<bxgi::DATLoaderEntry*>
{
public:
	using bxcf::VectorPool<bxgi::DATLoaderEntry*>::getEntryCount;

	DATLoaderFormat(void) : ContainerFormat(false) {}
	DATLoaderFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : ContainerFormat(strFilePathOrData, bStringIsFilePath, false) {}

	void										unload(void);

	std::vector<std::string>					getRelativeIDEPaths(void);
	std::vector<std::string>					getRelativeIPLPaths(void);

	std::vector<bxgi::IMGFormat*>				parseIMGFiles(std::string strGameDirectoryPath);
	std::vector<bxgi::IDEFormat*>				parseIDEFiles(std::string strGameDirectoryPath);
	std::vector<bxgi::IPLFormat*>				parseIPLFiles(std::string strGameDirectoryPath);

	template<class ManagerClass, class FormatClass>
	std::vector<FormatClass*>					parseFiles(std::string strGameDirectoryPath, bxgi::EDATLoaderEntryType eType1, bxgi::EDATLoaderEntryType eType2 = DAT_LOADER_UNKNOWN);

	uint32										getVersion(void);
	EGame										getGame(void);

	// ContainerFormat virtuals
	bxgi::DATLoaderEntry*						addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	bxgi::DATLoaderEntry*						addEntryViaData(std::string& strEntryName, std::string& strEntryData);
	bxgi::DATLoaderEntry*						replaceEntryViaFile(std::string& strEntryFilePath, std::string strExistingEntryName = "", std::string strNewEntryName = "");
	bxgi::DATLoaderEntry*						replaceEntryViaData(std::string& strEntryData, std::string strExistingEntryName, std::string strNewEntryName = "");

	void										exportSingle(bxcf::FormatEntry *pEntry, std::string& strFolderPath) {}
	void										exportMultiple(std::vector<bxcf::FormatEntry*>& vecIMGEntries, std::string& strFolderPath) {}
	void										exportAll(std::string& strFolderPath) {}

	std::vector<bxcf::FormatEntry*>				getAllEntries(void) { return (std::vector<bxcf::FormatEntry*>)(std::vector<bxcf::FormatEntry*>&)m_vecEntries; }
	std::vector<bxcf::FormatEntry*>&			getEntriesRef(void) { return (std::vector<bxcf::FormatEntry*>&)m_vecEntries; }

	void										swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2) {}

	uint32										getRawVersion(void) { return -1; }

	void										merge(std::string& strFilePath) {}
	void										split(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFilePathOut, uint32 uiFileVersionOut) {}

	// DATLoaderFormat continued

private:
	void										_unserialize(void);
	void										_serialize(void);

	void										unserializeLine(void);
};