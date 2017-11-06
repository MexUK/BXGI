#pragma once

#include "nsbxgi.h"
#include "Format/ContainerFormat.h"
#include "Pool/VectorPool.h"
#include "WTDEntry.h"
#include "d3d9.h"
#include <string>

class bxgi::IntermediateTextureFormat;

class bxgi::WTDFormat : public bxcf::ContainerFormat, public bxcf::VectorPool<bxgi::WTDEntry*>
{
public:
	using bxcf::VectorPool<bxgi::WTDEntry*>::getEntries;

	WTDFormat(void) : ContainerFormat(true, bxcf::LITTLE_ENDIAN) {}
	WTDFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxcf::ContainerFormat(strFilePathOrData, bStringIsFilePath, true, bxcf::LITTLE_ENDIAN) {}

	void											unload(void);

	virtual bxgi::WTDEntry*							addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	virtual bxgi::WTDEntry*							addEntryViaData(std::string& strEntryName, std::string& strEntryData);

	void											exportSingle(bxcf::FormatEntry *pEntry, std::string& strFolderPath);
	void											exportMultiple(std::vector<bxcf::FormatEntry*>& vecEntries, std::string& strFolderPath);
	void											exportAll(std::string& strFolderPath);

	bxgi::WTDEntry*									getEntryByName(std::string& strEntryName); // case-insensitive

	std::vector<bxcf::FormatEntry*>					getAllEntries(void);

	void											swapEntries(bxcf::FormatEntry *pEntry1, bxcf::FormatEntry *pEntry2);

	std::vector<bxgi::WTDEntry*>					getSelectedEntries(void);
	uint32											getSelectedEntryCount(void);

	bxgi::IntermediateTextureFormat*				convertToIntermediateFormat(void);

	static uint32									getFileHeaderFlagsFromSystemAndGraphicsStreamSizes(uint32 uiSystemStreamSize, uint32 uiGraphicsStreamSize);

	static std::string								getFourCCFromD3DFormat(D3DFORMAT d3dFormat);
	static D3DFORMAT								getD3DFormatFromFourCC(std::string strFourCC);

private:
	void											_unserialize(void);
	void											_serialize(void);

	static uint32									getCompactSize(uint32 uiSize);
	uint32											convertULongToOffset(uint32 uiValue);
	uint32											convertULongToDataOffset(uint32 uiValue);

	std::string										decompressWTDFormatData(uint32& uiSystemSegmentSize, uint32& uiGPUSegmentSize);
};