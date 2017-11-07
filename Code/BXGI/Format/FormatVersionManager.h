#pragma once

#include "nsbxgi.h"
#include "Pool/VectorPool.h"
#include "Object/Singleton.h"
#include "Format/EFileType.h"
#include <string>
#include <vector>

class bxgi::FormatVersionManager : public bxcf::Singleton<bxgi::FormatVersionManager>, public bxcf::VectorPool<bxgi::FormatVersion*>
{
public:
	void						init(void);
	void						initFormatVersions(void);

	FormatVersion*				addFormatVersion(bxcf::fileType::EFileType uiFileType, uint32 uiRawVersion, std::string& strVersionText);

	std::vector<std::string>	getEntriesVersionsText(bxcf::fileType::EFileType uiFileType = bxcf::fileType::EFileType::UNKNOWN);

	FormatVersion*				getEntryByVersionText(std::string& strVersionText);
};