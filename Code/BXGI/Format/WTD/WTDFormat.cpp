#include "WTDFormat.h"
#include "Static/File.h"
#include "Static/String.h"
#include "Static/Memory.h"
#include "Static/Path.h"
#include "Stream/DataWriter.h"
#include "WTDManager.h"
#include "Intermediate/Texture/IntermediateTextureFormat.h"
#include "Intermediate/Texture/IntermediateTexture.h"
#include "Intermediate/Texture/Data/IntermediateTextureMipmap.h"
#include "Compression/CompressionManager.h"
#include "Image/ImageManager.h"
#include "Stream/DataReader.h"
#include "Format/WTD/Raw/WTDFormat_Header1.h"
#include "Format/WTD/Raw/WTDFormat_Header2.h"
#include "Format/WTD/Raw/WTDEntry_RG.h"
#include "Engine/RAGE/RageManager.h"
#include "Image/ImageFile.h"
#include "Event/Events.h"
#include "../bxcf/Event/EEvent.h"
#include "Format/Image/BMP/BMPFormat.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

void					WTDFormat::unload(void)
{
	for (auto pWTDEntry : getEntries())
	{
		for (auto pWTDMipmap : pWTDEntry->getEntries())
		{
			delete pWTDMipmap;
		}
		//pWTDEntry->removeAllEntries();
		delete pWTDEntry;
	}
	getEntries().clear();
}

// add entry
WTDEntry*				WTDFormat::addEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	if (strEntryName == "")
	{
		strEntryName = Path::getFileName(strEntryFilePath);
	}

	ImageFile *pImageFile = ImageManager::loadImageFromFile(strEntryFilePath);

	WTDEntry *pEntry = new WTDEntry;
	pEntry->setEntryName(strEntryName);
	pEntry->setD3DFormat(D3DFMT_A8R8G8B8);
	pEntry->setRasterDataFormat(RASTERDATAFORMAT_BGRA32);
	pEntry->setImageSize(true, pImageFile->m_uiImageWidth);
	pEntry->setImageSize(false, pImageFile->m_uiImageHeight);
	pEntry->setLevels(1);
	pEntry->setTextureHash(0);

	WTDMipmap *pMipMap = new WTDMipmap(pEntry);
	pMipMap->setImageSize(true, pImageFile->m_uiImageWidth);
	pMipMap->setImageSize(false, pImageFile->m_uiImageHeight);
	pMipMap->setRasterData(pImageFile->m_strRasterDataBGRA32);
	pEntry->addEntry(pMipMap);

	addEntry(pEntry);
	
	return pEntry;
}

WTDEntry*				WTDFormat::addEntryViaData(string& strEntryName, string& strEntryData)
{
	return nullptr; // todo
}

// serialization
void					WTDFormat::_unserialize(void)
{
	DataReader *pDataReader = &m_reader;

	// header 1 and decompress whole file except first 12 bytes
	uint32 uiSystemSegmentSize, uiGPUSegmentSize;
	string strData = decompressWTDFormatData(uiSystemSegmentSize, uiGPUSegmentSize);
	pDataReader->setStreamType(DATA_STREAM_MEMORY); // todo - make this call DataReader::close() or something so that is closes the file handle
	pDataReader->setData(strData);
	pDataReader->setSeek(0);

	// header 2
	RG_WTDFormat_Header2 *pHeader2 = pDataReader->readStruct<RG_WTDFormat_Header2>();
	//pHeader2->m_uiHashTableOffset = String::swapEndian(pHeader2->m_uiHashTableOffset); // todo - is this code needed?
	pHeader2->m_uiHashTableOffset = convertULongToOffset(pHeader2->m_uiHashTableOffset);
	pHeader2->m_usTextureCount = (uint16) convertULongToOffset(pHeader2->m_usTextureCount);
	pHeader2->m_uiTextureListOffset = convertULongToOffset(pHeader2->m_uiTextureListOffset);

	// texture hashes
	pDataReader->setSeek(pHeader2->m_uiHashTableOffset);
	vector<uint32> vecTextureHashes = pDataReader->readUint32ArrayAsStdVector(pHeader2->m_usTextureCount);

	// info offsets
	pDataReader->setSeek(pHeader2->m_uiTextureListOffset);
	vector<uint32> vecInfoOffsets = pDataReader->readUint32ArrayAsStdVector(pHeader2->m_usTextureCount);
	for (uint32& uiInt : vecInfoOffsets)
	{
		uiInt = convertULongToOffset(uiInt);
	}

	// prepare stored container
	m_vecEntries.clear();
	m_vecEntries.resize(pHeader2->m_usTextureCount);

	// RG structs
	RG_WTDEntry *pWTDEntries_RG = new RG_WTDEntry[pHeader2->m_usTextureCount];
	pDataReader->readCString((uint8*)pWTDEntries_RG, pHeader2->m_usTextureCount * sizeof(RG_WTDEntry));

	// header for entries
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		RG_WTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pDataReader->setSeek(vecInfoOffsets[i]);
		Memory::copyAndDeleteUint8Array(pWTDEntry_RG, pDataReader->readCString(sizeof(RG_WTDEntry)), sizeof(RG_WTDEntry));
	}

	// convert ints to offsets
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		RG_WTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pWTDEntry_RG->m_uiBlockMapOffset2 = convertULongToOffset(pWTDEntry_RG->m_uiBlockMapOffset2);
		pWTDEntry_RG->m_uiNameOffset = convertULongToOffset(pWTDEntry_RG->m_uiNameOffset);

		//D3DFORMAT eD3DFormat = getD3DFormatFromFourCC(FileParser::get()->readString(4));
		//ERasterDataFormat ERasterDataFormatValue = ImageManager::getRasterDataFormatFromD3DFormat(eD3DFormat);

		pWTDEntry_RG->m_uiPrevTextureInfoOffset = convertULongToOffset(pWTDEntry_RG->m_uiPrevTextureInfoOffset);
		pWTDEntry_RG->m_uiNextTextureInfoOffset = convertULongToOffset(pWTDEntry_RG->m_uiNextTextureInfoOffset);

		pWTDEntry_RG->m_uiRawDataOffset = convertULongToDataOffset(pWTDEntry_RG->m_uiRawDataOffset);
	}

	// entry names
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		WTDEntry *pWTDEntry = new WTDEntry;
		m_vecEntries[i] = pWTDEntry;
		RG_WTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pDataReader->setSeek(pWTDEntry_RG->m_uiNameOffset);
		pWTDEntry->setEntryName(pDataReader->readStringUntilZero());
	}

	// copy RG structs to wrapper structs
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		WTDEntry *pWTDEntry = m_vecEntries[i];
		RG_WTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pWTDEntry->stripNameHeaderAndFooter();
		pWTDEntry->setImageSize(true, pWTDEntry_RG->m_usWidth);
		pWTDEntry->setImageSize(false, pWTDEntry_RG->m_usHeight);
		pWTDEntry->setD3DFormat(getD3DFormatFromFourCC((char*)&pWTDEntry_RG->m_ucD3DFormat));
		pWTDEntry->setRasterDataFormat(ImageManager::getRasterDataFormatFromD3DFormat(pWTDEntry->getD3DFormat()), false);
		pWTDEntry->setLevels(pWTDEntry_RG->m_ucLevels);
		pWTDEntry->setRawDataOffset(pWTDEntry_RG->m_uiRawDataOffset);
		pWTDEntry->setTextureHash(vecTextureHashes[i]);
	}

	// clean up
	delete[] pWTDEntries_RG;

	// raster data
	uint64 uiGraphicsStreamStartPosition = pDataReader->getSeek();
	for (uint32 i = 0; i < pHeader2->m_usTextureCount; i++)
	{
		WTDEntry *pWTDEntry = m_vecEntries[i];

		uint32 uiDataSize = WTDManager::getImageDataSize(pWTDEntry, false);

		uint32 uiSeek = uiSystemSegmentSize + pWTDEntry->getRawDataOffset();
		pDataReader->setSeek(uiSeek);

		uint32
			uiMipmapWidth = pWTDEntry->getImageSize(true),
			uiMipmapHeight = pWTDEntry->getImageSize(false);
		for (uint32 i = 0, j = pWTDEntry->getLevels(); i < j; i++)
		{
			// clamp to 16 bytes
			if (uiDataSize < 16)
			{
				if (pWTDEntry->getD3DFormat() == D3DFMT_DXT1 && uiDataSize < 8)
				{
					uiDataSize = 8;
				}
				else
				{
					uiDataSize = 16;
				}
			}

			// create mipmap
			WTDMipmap *pMipmap = new WTDMipmap(pWTDEntry);

			pMipmap->setRasterData(pDataReader->readString(uiDataSize));
			pMipmap->setImageSize(true, (uint16)uiMipmapWidth);
			pMipmap->setImageSize(false, (uint16)uiMipmapHeight);
			pWTDEntry->addEntry(pMipmap);

			// calculate data size and image size for next mipmap
			uiDataSize /= 4;
			uiMipmapWidth /= 2;
			uiMipmapHeight /= 2;
		}
	}
}

void					WTDFormat::_serialize(void)
{
	// todo

	DataWriter
		*pDataWriter = &m_writer,
		*pDataWriter2 = DataWriter::get(1);

	//EDataStreamType ePreviousDataStreamType = pDataWriter->getStreamType();
	pDataWriter2->setStreamType(DATA_STREAM_MEMORY);

	// store system stream
	uint32
		uiVTable = 0,
		uiBlockMapOffset = 0,
		uiParentDictionary = 0,
		uiUsageCount = 0,
		uiHashTableOffset = 0,
		uiTextureListOffset = 0
	;
	uint16
		usTextureCount = VectorPool::getEntryCount(),
		usUnknown1 = 0,
		usUnknown2 = 0,
		usUnknown3 = 0
	;

	pDataWriter2->writeUint32(uiVTable);

	pDataWriter2->writeUint32(RageManager::getPackedOffset(uiBlockMapOffset));
	pDataWriter2->writeUint32(uiParentDictionary);
	pDataWriter2->writeUint32(uiUsageCount);

	pDataWriter2->writeUint32(RageManager::getPackedOffset(uiHashTableOffset));
	pDataWriter2->writeUint16(usTextureCount);
	pDataWriter2->writeUint16(usUnknown1);

	pDataWriter2->writeUint32(RageManager::getPackedOffset(uiTextureListOffset));
	pDataWriter2->writeUint16(usUnknown2);
	pDataWriter2->writeUint16(usUnknown3);

	// store texture hashes
	for (auto pWTDEntry : getEntries())
	{
		pDataWriter2->writeUint32(pWTDEntry->getTextureHash());
	}

	// store texture info
	for (uint32 i = 0; i < usTextureCount; i++)
	{
		uint32
			uiVTable2 = 0,
			uiBlockMapOffset2 = 0,
			uiUnknown4 = 0,
			uiUnknown5 = 0,
			uiUnknown6 = 0,
			uiNameOffset = 0,
			uiUnknown7 = 0,
			uiPrevTextureInfoOffset = 0,
			uiNextTextureInfoOffset = 0,
			uiRawDataOffset = 0,
			uiUnknown14 = 0
		;
		uint16
			usWidth = 0,
			usHeight = 0,
			usStrideSize = 0
		;
		uint8
			ucType = 0,
			ucLevels = 0
		;
		float
			fUnknown8 = 0.0f,
			fUnknown9 = 0.0f,
			fUnknown10 = 0.0f,
			fUnknown11 = 0.0f,
			fUnknown12 = 0.0f,
			fUnknown13 = 0.0f
		;
		D3DFORMAT eD3DFormat = D3DFMT_DXT1;

		pDataWriter2->writeUint32(uiVTable2);

		pDataWriter2->writeUint32(RageManager::getPackedOffset(uiBlockMapOffset2));

		pDataWriter2->writeUint32(uiUnknown4);
		pDataWriter2->writeUint32(uiUnknown5);
		pDataWriter2->writeUint32(uiUnknown6);

		pDataWriter2->writeUint32(RageManager::getPackedOffset(uiNameOffset));

		pDataWriter2->writeUint32(uiUnknown7);

		pDataWriter2->writeUint16(usWidth);
		pDataWriter2->writeUint16(usHeight);
		pDataWriter2->writeString(getFourCCFromD3DFormat(eD3DFormat));

		pDataWriter2->writeUint16(usStrideSize);
		pDataWriter2->writeUint8(ucType);
		pDataWriter2->writeUint8(ucLevels);

		pDataWriter2->writeFloat32(fUnknown8);
		pDataWriter2->writeFloat32(fUnknown9);
		pDataWriter2->writeFloat32(fUnknown10);
		pDataWriter2->writeFloat32(fUnknown11);
		pDataWriter2->writeFloat32(fUnknown12);
		pDataWriter2->writeFloat32(fUnknown13);

		pDataWriter2->writeUint32(RageManager::getPackedOffset(uiPrevTextureInfoOffset));
		pDataWriter2->writeUint32(RageManager::getPackedOffset(uiNextTextureInfoOffset));

		pDataWriter2->writeUint32(RageManager::getPackedOffset(uiRawDataOffset));

		pDataWriter2->writeUint32(uiUnknown14);
	}

	// store texture names
	for (auto pWTDEntry : getEntries())
	{
		string strEntryName = pWTDEntry->getEntryName();
		strEntryName.append("\0", 1);
		pDataWriter2->writeStringRef(strEntryName);
	}

	// store graphics stream
	uint32 uiSystemStreamSize = pDataWriter2->getData().length();
	for (auto pWTDEntry : getEntries())
	{
		for (auto pMipmap : pWTDEntry->getEntries())
		{
			pDataWriter2->writeStringRef(pMipmap->getRasterData());
		}
	}
	uint32 uiGraphicsStreamSize = pDataWriter2->getData().length() - uiSystemStreamSize;

	//string strWTDFileData = pDataWriter->getData();
	//pDataWriter->resetData();
	//pDataWriter->setStreamType(ePreviousDataStreamType);
	//pDataWriter->setSeek(0);

	uint32
		uiMagicNumber = 0x52534305, // R S C 0x05
		uiType = 0x08,
		uiFlags = getFileHeaderFlagsFromSystemAndGraphicsStreamSizes(uiSystemStreamSize, uiGraphicsStreamSize);
	;
	bool
		bBigEndian = false
	;
	string strHeader12B = 
		String::packUint32(uiMagicNumber, bBigEndian) +
		String::packUint32(uiType, bBigEndian) +
		String::packUint32(uiFlags, bBigEndian);
	pDataWriter->writeString(strHeader12B + CompressionManager::compressZLib(pDataWriter2->getData()));

	pDataWriter2->reset();
}

// convert format
IntermediateTextureFormat*		WTDFormat::convertToIntermediateFormat(void)
{
	IntermediateTextureFormat *pGeneralTextureFile = new IntermediateTextureFormat;

	for (WTDEntry *pWTDEntry : getEntries())
	{
		IntermediateTexture *pGeneralTexture = new IntermediateTexture;

		Vec2u16 vecImageSize;
		vecImageSize.x = pWTDEntry->getImageSize(true);
		vecImageSize.y = pWTDEntry->getImageSize(false);

		pGeneralTexture->setName(pWTDEntry->getEntryName());
		pGeneralTexture->setRasterDataFormat(ImageManager::getRasterDataFormatFromD3DFormat(pWTDEntry->getD3DFormat()));
		pGeneralTexture->setSize(vecImageSize);

		for (WTDMipmap *pWTDMipmap : pWTDEntry->getEntries())
		{
			IntermediateTextureMipmap *pGeneralMipmap = new IntermediateTextureMipmap;

			Vec2u16 vecMipmapSize;
			vecMipmapSize.x = pWTDMipmap->getImageSize(true);
			vecMipmapSize.y = pWTDMipmap->getImageSize(false);

			pGeneralMipmap->setRasterData(pWTDMipmap->getRasterData());
			pGeneralMipmap->setSize(vecMipmapSize);

			pGeneralTexture->addEntry(pGeneralMipmap);
		}

		pGeneralTextureFile->addEntry(pGeneralTexture);
	}

	return pGeneralTextureFile;
}

// stream sizes
uint32				WTDFormat::getFileHeaderFlagsFromSystemAndGraphicsStreamSizes(uint32 uiSystemStreamSize, uint32 uiGraphicsStreamSize)
{
	return (getCompactSize(uiSystemStreamSize) & 0x7FFF) | (getCompactSize(uiGraphicsStreamSize) & 0x7FFF) << 15 | 3 << 30;
}

uint32				WTDFormat::getCompactSize(uint32 uiSize)
{
	// uiSize must be a multiple of 256
	uiSize >>= 8;
	uint32 i = 0;
	while ((uiSize % 2) == 0 && uiSize >= 32 && i < 15)
	{
		i++;
		uiSize >>= 1;
	}
	return ((i & 0x0F) << 11) | (uiSize & 0x7FF);
}

// compression
string						WTDFormat::decompressWTDFormatData(uint32& uiSystemSegmentSize, uint32& uiGPUSegmentSize)
{
	DataReader *pDataReader = &m_reader;

	RG_WTDFormat_Header1 *pHeader1 = pDataReader->readStruct<RG_WTDFormat_Header1>();

	/*
	todo - is this code needed?
	header1.m_uiFlags = String::swapEndian(header1.m_uiFlags);
	header1.m_uiMagicNumber = String::swapEndian(header1.m_uiMagicNumber);
	header1.m_uiType = String::swapEndian(header1.m_uiType);
	*/

	uiSystemSegmentSize = (pHeader1->m_uiFlags & 0x7FF) << (((pHeader1->m_uiFlags >> 11) & 0xF) + 8);
	uiGPUSegmentSize = ((pHeader1->m_uiFlags >> 15) & 0x7FF) << (((pHeader1->m_uiFlags >> 26) & 0xF) + 8);

	delete pHeader1;

	return CompressionManager::decompressZLib(pDataReader->readRemaining(), uiSystemSegmentSize + uiGPUSegmentSize);
}

// offsets
uint32			WTDFormat::convertULongToOffset(uint32 uiValue)
{
	if (uiValue == 0)
	{
		return 0;
	}

	return uiValue & 0x0FFFFFFF;
}

uint32			WTDFormat::convertULongToDataOffset(uint32 uiValue)
{
	if (uiValue == 0)
	{
		return 0;
	}

	return uiValue & 0x0FFFFFFF;
}

// D3D Formats
D3DFORMAT				WTDFormat::getD3DFormatFromFourCC(string strFourCC)
{
	if (strFourCC == "DXT1")
	{
		return D3DFMT_DXT1;
	}
	else if (strFourCC == "DXT3")
	{
		return D3DFMT_DXT3;
	}
	else if (strFourCC == "DXT5")
	{
		return D3DFMT_DXT5;
	}
	return D3DFMT_UNKNOWN;
}

string					WTDFormat::getFourCCFromD3DFormat(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return "DXT1";
	case D3DFMT_DXT3:	return "DXT3";
	case D3DFMT_DXT5:	return "DXT5";
	}
	return "Unkn";
}

void					WTDFormat::exportMultiple(vector<FormatEntry*>& vecEntries, string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getFilePath());
	if (!openFile())
	{
		return;
	}

	for (WTDEntry *pWTDEntry : (vector<WTDEntry*>&)vecEntries)
	{
		/*
		todo
		if (!pWTDEntry->canBeRead())
		{
			continue;
		}
		*/

		File::storeFile(strFolderPath + pWTDEntry->getEntryName(), readEntryContent(getIndexByEntry(pWTDEntry)), false, true);

		Events::trigger(TASK_PROGRESS);
	}

	m_reader.close();
}

void					WTDFormat::exportAll(string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getFilePath());
	if (!openFile())
	{
		return;
	}

	for (WTDEntry *pWTDEntry : (vector<WTDEntry*>&)getEntries())
	{
		WTDMipmap *pMipmap = pWTDEntry->getEntryByIndex(0);

		BMPFormat *pBMPFile = new BMPFormat;
		pBMPFile->setWidth(pMipmap->getImageSize(true));
		pBMPFile->setHeight(pMipmap->getImageSize(false));
		pBMPFile->setBPP(32);

		pBMPFile->setRasterData(pMipmap->getRasterDataBGRA32());
		pBMPFile->swapRows();

		string strBMPFilePath = strFolderPath + "/" + pWTDEntry->getEntryName() + ".BMP";
		strBMPFilePath = Path::getNextFileName(strBMPFilePath, 1, "-Mipmap");

		pBMPFile->setBMPVersion(3);
		pBMPFile->serialize(strBMPFilePath);

		pBMPFile->unload();
		delete pBMPFile;

		Events::trigger(TASK_PROGRESS);
	}

	m_reader.close();
}

WTDEntry*				WTDFormat::getEntryByName(string& strEntryName)
{
	string
		strEntryName2 = String::toUpperCase(strEntryName),
		strEntryName2NoExt = String::toUpperCase(Path::removeFileExtension(strEntryName));
	for (WTDEntry *pWTDEntry : getEntries())
	{
		if (strEntryName2 == String::toUpperCase(pWTDEntry->getEntryName()))
		{
			return pWTDEntry;
		}
		if (strEntryName2NoExt == String::toUpperCase(Path::removeFileExtension(pWTDEntry->getEntryName())))
		{
			return pWTDEntry;
		}
	}
	return nullptr;
}