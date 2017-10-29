#pragma once

#include "nsbxgi.h"
#include "Format/Format.h"
#include "Pool/VectorPool.h"
#include "EDATLoaderEntryType.h"
#include <string>
#include <vector>

class bxgi::DATLoaderEntry;
class bxgi::IMGFormat;
class bxgi::IDEFormat;
class bxgi::IPLFormat;

class bxgi::DATLoaderFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::DATLoaderEntry*>
{
public:
	using bxcf::VectorPool<bxgi::DATLoaderEntry*>::getEntryCount;

	DATLoaderFormat(void) : Format(false) {}
	DATLoaderFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxcf::Format(strFilePathOrData, bStringIsFilePath, false) {}

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

private:
	void										_unserialize(void);
	void										_serialize(void) {} // todo

	void										unserializeLine(void);
};