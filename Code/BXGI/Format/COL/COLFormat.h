#pragma once

#include "nsbxgi.h"
#include "Format/Format.h"
#include "Pool/VectorPool.h"
#include "ECOLVersion.h"
#include "COLEntry.h"
#include <string>
#include <vector>

class bxgi::COLVersion;

class bxgi::COLFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::COLEntry*>
{
public:
	using bxcf::VectorPool<bxgi::COLEntry*>::getEntryCount;

	COLFormat(void) : Format(true) {}
	COLFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxcf::Format(strFilePathOrData, bStringIsFilePath, true, bxcf::LITTLE_ENDIAN) {}

	void							unload(void);

	void							_readMetaData(void) {} // todo

	void							setCOLVersion(ECOLVersion uiCOLVersion);

	std::vector<std::string>		getModelNames(void);
	bxgi::COLEntry*					getEntryByName(std::string strName);
	std::vector<bxgi::COLEntry*>	getEntriesByModelName(std::string strModelName);

	void							sort(void);
	static bool						sortFunction(bxgi::COLEntry *pCOLEntry1, bxgi::COLEntry *pCOLEntry2);

private:
	void							_unserialize(void);
	void							_serialize(void);

	void							checkToFixPEDSDotCOL(void);
};