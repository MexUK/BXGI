#include "IMGEntry.h"
#include "IMGFormat.h"
#include "IMGManager.h"
#include "Static/Path.h"
#include "Static/String.h"
#include "Static/File.h"
#include "Static/Debug.h"
#include "Compression/CompressionManager.h"
#include "Localization/LocalizationManager.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersionManager.h"
#include "Engine/RAGE/RageManager.h"
#include "Engine/RAGE/RageResourceTypeManager.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version1Or2.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version3.h"
#include "Format/IMG/Fastman92/IMGEntry_Fastman92.h"
#include "Format/COL/COLManager.h"
#include "Format/COL/COLVersionManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;
using namespace bxcf::fileType;

IMGEntry::IMGEntry(void) :
	m_pIMGFile(nullptr),
	m_uiEntryOffset(0),
	m_uiEntrySize(0),
	m_bNewEntry(false),
	m_bReplacedEntry(false),
	m_bProtectedEntry(false),
	m_uiFileCreationDate(0),
	m_ECompressionAlgorithm(COMPRESSION_NONE),
	m_uiCompressionLevel(0),
	m_bIsEncrypted(false),
	m_pRageResourceType(nullptr),
	m_uiUncompressedSize(0),
	m_uiFlags(0),
	m_uiRawVersion(0)
{
}

IMGEntry::IMGEntry(IMGFormat *pIMGFile) :
	m_pIMGFile(pIMGFile),
	m_uiEntryOffset(0),
	m_uiEntrySize(0),
	m_bNewEntry(false),
	m_bReplacedEntry(false),
	m_bProtectedEntry(false),
	m_uiFileCreationDate(0),
	m_ECompressionAlgorithm(COMPRESSION_NONE),
	m_uiCompressionLevel(0),
	m_bIsEncrypted(false),
	m_pRageResourceType(nullptr),
	m_uiUncompressedSize(0),
	m_uiFlags(0),
	m_uiFileType(bxcf::fileType::UNKNOWN)
{
}

void					IMGEntry::setEntrySize(uint32 uiEntrySize)
{
	m_uiEntrySize = uiEntrySize;
	if (getIMGFile()->getVersion() == IMG_3)
	{
		m_uiFlags = (m_uiFlags & 63488) | (uint16)getVersion3IMGSizeDeduction(uiEntrySize);
	}
}

uint32			IMGEntry::getPaddedEntrySize(void)
{
	return (uint32) (ceil(((float32) getEntrySize()) / 2048.0f) * 2048.0f);
}

void					IMGEntry::setEntryData(string strEntryData, bool bIsNew)
{
	//setEntryOffset(getIMGFile()->getNextEntryOffset()); // todo - this line is needed but getNextEntryOffset isnt defined yet
	uint32 uiFUncompressedSize = (uint32)strEntryData.length();
	if(!bIsNew)
	{
		setReplacedEntry(true);
	}

	strEntryData = String::zeroPad(strEntryData, IMGEntry::getEntryDataPadLength(strEntryData.length()));
	if (getIMGFile()->getVersion() == IMG_FASTMAN92)
	{
		setUncompressedSize(uiFUncompressedSize);

		if (isCompressed())
		{
			// compressed
			switch (getCompressionAlgorithmId())
			{
			case COMPRESSION_ZLIB:
				strEntryData = CompressionManager::compressZLib(strEntryData, getCompressionLevel());
				break;
			case COMPRESSION_LZ4:
				strEntryData = CompressionManager::compressLZ4(strEntryData, getCompressionLevel());
				break;
			}
		}
		else
		{
			// not compressed
		}
	}
	/*
	// todo - add getPlatform() to IMGFormat and uncomment this code
	else if (getIMGFile()->getPlatform() == PLATFORM_XBOX)
	{
		// the entry data is compressed with LZO 1X 999 compression
		bool bBigEndian = false;
		string strOutputData = "";
		const uint32 uiBlockSize = 131072;

		for (uint32 i = 0, uiBlockCount = ceil((float32)strEntryData.length() / (float32)uiBlockSize); i < uiBlockCount; i++)
		{
			string strCompressedData;
			if (i == (uiBlockCount - 1))
			{
				strCompressedData = CompressionManager::compressLZO1X999(strEntryData.substr(i * uiBlockSize));
			}
			else
			{
				strCompressedData = CompressionManager::compressLZO1X999(strEntryData.substr(i * uiBlockSize, uiBlockSize));
			}

			uint32 uiUnknown1 = 4;
			uint32 uiUncompressedBlockSize = strEntryData.length();
			uint32 uiCompressedBlockSize = strCompressedData.length();

			strOutputData += String::packUint32(uiUnknown1, bBigEndian);
			strOutputData += String::packUint32(uiUncompressedBlockSize, bBigEndian);
			strOutputData += String::packUint32(uiCompressedBlockSize, bBigEndian);

			strOutputData += strCompressedData;
		}

		uint32 uiMagicNumber = 0x67A3A1CE;
		uint32 uiChecksum = 0;
		uint32 uiCompressionDataSize = strOutputData.length();

		string strOutputData2 = "";
		strOutputData2 += String::packUint32(uiMagicNumber, bBigEndian);
		strOutputData2 += String::packUint32(uiChecksum, bBigEndian);
		strOutputData2 += String::packUint32(uiCompressionDataSize, bBigEndian);
		
		strEntryData = strOutputData2 + strOutputData;
	}
	*/
	setEntrySize((uint32)strEntryData.length());
	File::storeFileSubContent(getIMGFile()->getFilePath(), strEntryData, getEntryOffset());
}

string					IMGEntry::getEntryData(void)
{
	string strEntryData = File::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset(), getEntrySize(), true);
	bool bBigEndian = false;

	if (getIMGFile()->getVersion() == IMG_FASTMAN92)
	{
		if (isCompressed())
		{
			// compressed
			switch (getCompressionAlgorithmId())
			{
			case COMPRESSION_ZLIB:	return CompressionManager::decompressZLib(strEntryData, getUncompressedSize());
			case COMPRESSION_LZ4:	return CompressionManager::decompressLZ4(strEntryData, getUncompressedSize());
			default:					return "";
			}
		}
		else
		{
			// not compressed
			return strEntryData;
		}
	}
	else if (String::unpackUint32(strEntryData.substr(0, 4), bBigEndian) == 0x67A3A1CE)
	{
		// the entry data is compressed with LZO 1X 999 compression
		uint32 uiSeek = 0;

		uint32 uiMagicNumber = String::unpackUint32(strEntryData.substr(uiSeek, 4), bBigEndian);
		uint32 uiChecksum = String::unpackUint32(strEntryData.substr(uiSeek + 4, 4), bBigEndian);
		uint32 uiCompressionDataSize = String::unpackUint32(strEntryData.substr(uiSeek + 8, 4), bBigEndian);
		uiSeek += 12;

		string strUncompressedData = "";
		uint32 uiByteCountRead = 0;
		while (uiByteCountRead < uiCompressionDataSize)
		{
			uint32 uiUnknown1 = String::unpackUint32(strEntryData.substr(uiSeek, 4), bBigEndian);
			uint32 uiUncompressedBlockSize = String::unpackUint32(strEntryData.substr(uiSeek + 4, 4), bBigEndian);
			uint32 uiCompressedBlockSize = String::unpackUint32(strEntryData.substr(uiSeek + 8, 4), bBigEndian);

			string strCompressedBlock = strEntryData.substr(uiSeek + 12, uiCompressedBlockSize);
			strUncompressedData += CompressionManager::decompressLZO1X(strCompressedBlock, uiUncompressedBlockSize);

			uiSeek += 12 + uiCompressedBlockSize;
			uiByteCountRead += 12 + uiCompressedBlockSize;
		}

		return strUncompressedData;
	}
	else
	{
		return strEntryData;
	}
}

string					IMGEntry::getEntrySubData(uint32 uiStart, uint32 uiLength)
{
	if (getIMGFile()->getVersion() == IMG_FASTMAN92 && isCompressed())
	{
		// compressed
		const uint8 ucZLibBlockSize = 16;
		uint32 uiStart2 = (uint32) floor((float32)uiStart / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiLength2 = (uint32) ceil((float32)uiLength / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiStartOffset = uiStart - uiStart2;
		/*
		Debug::log("uiStart: " + String::toString(uiStart));
		Debug::log("uiLength: " + String::toString(uiLength));
		Debug::log("uiStart2: " + String::toString(uiStart2));
		Debug::log("uiLength2: " + String::toString(uiLength2));
		Debug::log("uiStartOffset: " + String::toString(uiStartOffset));
		*/
		string strEntrySubData = File::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset() + uiStart2, uiLength2, true);
		//strEntrySubData = IMGManager::decompressZLib(strEntrySubData, uiLength2);
		switch (getCompressionAlgorithmId())
		{
		case COMPRESSION_ZLIB:	return CompressionManager::decompressZLib(strEntrySubData, getUncompressedSize());
		case COMPRESSION_LZ4:	return CompressionManager::decompressLZ4(strEntrySubData, getUncompressedSize());
		}
		return strEntrySubData.substr(uiStartOffset, uiLength);
	}
	else
	{
		// not compressed
		return File::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset() + uiStart, uiLength, true);
	}
}

void					IMGEntry::saveEntry(string strFilePath)
{
	File::storeFile(strFilePath, getEntryData(), false, true);
}

void					IMGEntry::saveEntryByMemory(string strFilePath, string& strEntryData)
{
	File::storeFile(strFilePath, strEntryData, false, true);
}

uint32			IMGEntry::getEntryDataPadLength(uint32 uiUnpaddedDataLength)
{
	return (uint32)(ceil((float32)uiUnpaddedDataLength / (float32)2048.0f) * (float32)2048.0f);
}

void					IMGEntry::setRWVersionByVersionCC(uint32 uiRWVersionCC)
{
	// todo
	//setRWVersion(RWManager::get()->getVersionManager()->getEntryByVersionCC(uiRWVersionCC));
}

void					IMGEntry::setRageResourceTypeByIdentifier(uint32 uiResourceType)
{
	setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByIdentifier(uiResourceType));
}

uint32			IMGEntry::getVersion3IMGSizeDeduction(uint32 uiDataLength)
{
	return 2048 - (uiDataLength % 2048);
}

bool					IMGEntry::isRWFile(void)
{
	return m_uiFileType == TEXTURE || m_uiFileType == MODEL;
}

bool					IMGEntry::isModelFile(void)
{
	return m_uiFileType == MODEL;
}

bool					IMGEntry::isTextureFile(void)
{
	return m_uiFileType == TEXTURE;
}

bool					IMGEntry::isCollisionFile(void)
{
	return m_uiFileType == COLLISION;
}

/*
string					IMGEntry::getEntryDecompressedData(void)
{

}
string					IMGEntry::getEntryCompressedData(void)
{

}
*/

void					IMGEntry::applyCompression(ECompressionAlgorithm ECompressionAlgorithmValue, uint32 uiCompressionLevel)
{
	string strEntryData = getEntryData();
	setCompressionAlgorithmId(ECompressionAlgorithmValue);
	setCompressionLevel(uiCompressionLevel);
	setEntryData(strEntryData);
}

bool					IMGEntry::doesHaveUnknownVersion(void)
{
	if (isEncrypted() || getIMGFile()->isEncrypted())
	{
		return false;
	}

	switch (m_uiFileType)
	{
	case TEXTURE:
	case MODEL:
	case COLLISION:
	case ANIMATION:
		return m_uiRawVersion == 0;
	}
	return false;
}

IMGEntry*				IMGEntry::clone(IMGFormat *pIMGFile)
{
	IMGEntry *pClonedIMGEntry = new IMGEntry(pIMGFile);

	pClonedIMGEntry->setEntryOffsetInSectors(getEntryOffsetInSectors());
	pClonedIMGEntry->setEntrySize(getEntrySize());
	pClonedIMGEntry->setEntryName(getEntryName());
	
	if (isRWFile())
	{
		pClonedIMGEntry->setRWVersion(getRWVersion());
	}
	else if (isCollisionFile())
	{
		pClonedIMGEntry->setCOLVersion(getCOLVersion());
	}

	if (pIMGFile->getVersion() == IMG_3)
	{
		pClonedIMGEntry->setRageResourceType(getRageResourceType());
		pClonedIMGEntry->setFlags(getFlags());
	}
	else if (pIMGFile->getVersion() == IMG_FASTMAN92)
	{
		pClonedIMGEntry->setUncompressedSize(getUncompressedSize());
		pClonedIMGEntry->setFlags(getFlags());
	}

	pClonedIMGEntry->setNewEntry(isNewEntry());
	pClonedIMGEntry->setReplacedEntry(isReplacedEntry());
	pClonedIMGEntry->setProtectedEntry(isProtectedEntry());
	pClonedIMGEntry->setFileCreationDate(getFileCreationDate());
	pClonedIMGEntry->setCompressionAlgorithmId(getCompressionAlgorithmId());

	return pClonedIMGEntry;
}

bool				IMGEntry::canBeRead(void)
{
	if (getIMGFile()->isEncrypted())
	{
		return false;
	}

	if (isEncrypted())
	{
		return false;
	}

	if (isCompressed() && getCompressionAlgorithmId() == COMPRESSION_UNKNOWN)
	{
		return false;
	}

	return true;
}

string				IMGEntry::getVersionText(void)
{
	if (getIMGFile()->getVersion() == IMG_3)
	{
		// IMG version 3
		if (getRageResourceType() == nullptr)
		{
			return LocalizationManager::get()->getTranslatedText("Window_Main_Combo_RWVersion_Unknown");
		}
		return getRageResourceType()->getResourceName();
	}
	else
	{
		// IMG version 1, 2 or fastman92
		switch (m_uiFileType)
		{
		case TEXTURE:
		case MODEL:
			// RW file
			return RWManager::get()->getVersionManager()->getVersionText(m_uiRawVersion);
		case COLLISION:
			// COL file
			return COLManager::get()->getVersionManager()->getVersionText(m_uiRawVersion);
		case ANIMATION:
			// IFP file
			return "IFP " + String::toString(m_uiRawVersion);
		}
	}
	return "Unknown";
}