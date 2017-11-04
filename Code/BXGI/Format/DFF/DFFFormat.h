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

	bxgi::IntermediateModelFormat*									convertToIntermediateModelFormat(void);
	std::vector<std::string>										getModelNames(void);
};