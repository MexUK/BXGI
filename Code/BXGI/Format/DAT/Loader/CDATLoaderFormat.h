#pragma once

#include "nsbxgi.h"
#include "Format/Format.h"
#include "Pool/VectorPool.h"
#include "EDATLoaderEntryType.h"
#include <string>
#include <vector>

class bxgi::CDATLoaderEntry;
class bxgi::CIMGFormat;
class bxgi::CIDEFormat;
class bxgi::CIPLFormat;

class bxgi::CDATLoaderFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::CDATLoaderEntry*>
{
public:
	CDATLoaderFormat(void) : Format(false) {}
	
	void									unload(void);

	std::vector<std::string>				getRelativeIDEPaths(void);
	std::vector<std::string>				getRelativeIPLPaths(void);

	std::vector<bxgi::CIMGFormat*>				parseIMGFiles(std::string strGameDirectoryPath);
	std::vector<bxgi::CIDEFormat*>				parseIDEFiles(std::string strGameDirectoryPath);
	std::vector<bxgi::CIPLFormat*>				parseIPLFiles(std::string strGameDirectoryPath);

	template<class ManagerClass, class FormatClass>
	std::vector<FormatClass*>				parseFiles(std::string strGameDirectoryPath, bxgi::EDATLoaderEntryType eType1, bxgi::EDATLoaderEntryType eType2 = DAT_LOADER_UNKNOWN);

private:
	void									unserialize(void);
	void									serialize(void) {} // todo

	void									unserializeLine(void);
};