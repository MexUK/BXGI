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

	void															exportMultiple(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFolderPath);
	void															exportAll(std::string& strFolderPath);

	bxcf::FormatEntry*												getEntryByName(std::string& strEntryName); // case-insensitive

	std::vector<bxcf::FormatEntry*>									getAllEntries(void);

	void															swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2) {} // todo

	bxgi::IntermediateModelFormat*									convertToIntermediateModelFormat(void);
	std::vector<std::string>										getModelNames(void);
};