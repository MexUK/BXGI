#pragma once

#include "nsbxgi.h"
#include "Format/RW/RWFormat.h"
#include "Intermediate/Model/IntermediateModelFormat.h"
#include "Format/RW/Sections/RWSection_TextureNative.h"
#include <string>
#include <vector>

class bxgi::DFFFormat : public bxgi::RWFormat
{
public:
	DFFFormat(void) {}
	DFFFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxgi::RWFormat(strFilePathOrData, bStringIsFilePath) {}

	virtual bxgi::RWSection_TextureNative*							addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "") { return nullptr; }
	virtual bxgi::RWSection_TextureNative*							addEntryViaData(std::string& strEntryName, std::string& strEntryData) { return nullptr; }

	void															exportSingle(bxcf::FormatEntry *pEntry, std::string& strFolderPath);
	void															exportMultiple(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFolderPath);
	void															exportAll(std::string& strFolderPath);

	bxcf::FormatEntry*												getEntryByName(std::string& strEntryName); // case-insensitive

	std::vector<bxcf::FormatEntry*>									getAllEntries(void);
	std::vector<bxcf::FormatEntry*>&								getEntriesRef(void) { return (std::vector<bxcf::FormatEntry*>&)m_vecTextures; }

	void															swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2) {} // todo

	void															merge(std::string& strFilePath) {} // todo
	void															split(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFilePathOut, uint32 uiFileVersionOut) {} // todo



	bxgi::IntermediateModelFormat*									convertToIntermediateModelFormat(void);
	std::vector<std::string>										getModelNames(void);

private:
	std::vector<bxcf::FormatEntry*>									m_vecTextures;
};