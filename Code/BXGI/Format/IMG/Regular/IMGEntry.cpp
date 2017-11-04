#include "IMGEntry.h"
#include "Format/IMG/Regular/IMGManager.h"
#include "Format/IMG/Regular/IMGFormatVersion3.h"
#include "Static/Path.h"
#include "Static/File.h"
#include "Static/Debug.h"
#include "Compression/CompressionManager.h"
#include "Localization/LocalizationManager.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersionManager.h"
#include "Engine/RAGE/RageManager.h"
#include "Engine/RAGE/RageResourceTypeManager.h"
#include "Format/COL/COLManager.h"
#include "Format/COL/COLVersionManager.h"
#include "Format/GameFormat.h"

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
	m_uiCompressionAlgorithm(COMPRESSION_NONE),
	m_uiEncryptionAlgorithm(ENCRYPTION_NONE),
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
	m_uiCompressionAlgorithm(COMPRESSION_NONE),
	m_uiEncryptionAlgorithm(ENCRYPTION_NONE),
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

uint32					IMGEntry::getPaddedEntrySize(void)
{
	return Math::convertSectorsToBytes(Math::convertBytesToSectors(m_uiEntrySize));
}

uint32					IMGEntry::getUndecryptedEntryOffset(void)
{
	return m_uiEntryOffset - (((IMGFormatVersion3*)m_pIMGFile)->getUndecryptedPositionOffset());
}

void					IMGEntry::unserializeRWVersion(DataReader *pDataReader, string strFilePath, string& strUncompressedEntryData)
{
	bool
		bUseNewDataReader = false,
		bUseStringAsData = strUncompressedEntryData != "";

	if (!bUseStringAsData)
	{
		bUseNewDataReader = pDataReader == nullptr;

		if (strFilePath == "")
		{
			strFilePath = m_pIMGFile->getFilePath();
		}

		if (bUseNewDataReader)
		{
			strFilePath = Path::replaceFileExtensionWithCase(strFilePath, "IMG");

			pDataReader = new DataReader;
			pDataReader->setStreamType(DATA_STREAM_FILE);
			pDataReader->setFilePath(strFilePath);
			pDataReader->open(true);
		}
	}

	bool bEntryIsCompressed;
	EFileType uiFileType;
	string strVersionCharacter;

	uiFileType = GameFormat::getRWFileType(getEntryExtension());
	setFileType(uiFileType);

	bEntryIsCompressed = isCompressed();
	if (bEntryIsCompressed)
	{
		strUncompressedEntryData = getEntryData(); // todo - don't modify this string reference
	}

	switch (uiFileType)
	{
	case MODEL:
	case TEXTURE:
		// RW file
		if (bEntryIsCompressed)
		{
			// RW file - compressed
			if (strUncompressedEntryData.length() >= 12)
			{
				setRawVersion(String::unpackUint32(strUncompressedEntryData.substr(8, 4), false));
			}
			else
			{
				setRawVersion(0);
			}
		}
		else
		{
			// RW file - not compressed
			if (bUseStringAsData)
			{
				setRawVersion(String::unpackUint32(strUncompressedEntryData.substr(8, 4), false));
			}
			else
			{
				pDataReader->setSeek(getEntryOffset() + 8);
				setRawVersion(pDataReader->readUint32());
			}
		}
		break;
	case COLLISION:
		// COL file
		if (bEntryIsCompressed)
		{
			// COL file - compressed
			if (strUncompressedEntryData.length() >= 4)
			{
				strVersionCharacter = strUncompressedEntryData.substr(3, 1);
			}
		}
		else
		{
			// COL file - not compressed
			if (bUseStringAsData)
			{
				strVersionCharacter = strUncompressedEntryData.substr(3, 1);
			}
			else
			{
				pDataReader->setSeek(getEntryOffset() + 3);
				strVersionCharacter = pDataReader->readString(1);
			}
		}

		if (strVersionCharacter.length())
		{
			if (strVersionCharacter == "L")
			{
				setRawVersion(1);
			}
			else
			{
				setRawVersion(String::toUint32(strVersionCharacter));
			}
		}
		else
		{
			setRawVersion(0);
		}

		break;
	case ANIMATION:
		// IFP file (animation)
		if (bUseStringAsData)
		{
			strVersionCharacter = strUncompressedEntryData.substr(3, 1);
		}
		else
		{
			pDataReader->setSeek(getEntryOffset() + 3);
			strVersionCharacter = pDataReader->readString(1);
		}

		if (strVersionCharacter == "K")
		{
			setRawVersion(1);
		}
		else
		{
			setRawVersion(String::toUint32(strVersionCharacter) - 1);
		}

		break;
	}

	if (bUseNewDataReader)
	{
		delete pDataReader;
	}
}

void					IMGEntry::unserializeResourceType(DataReader *pDataReader)
{
	m_uiFileType = GameFormat::getRageFileType(getEntryExtension());
}

void					IMGEntry::setEntryData(string& strEntryData, bool bIsNew)
{
	//setEntryOffset(getIMGFile()->getNextEntryOffset()); // todo - this line is needed but getNextEntryOffset isnt defined yet
	
	uint32 uiUncompressedSize = (uint32)strEntryData.length();

	if(!bIsNew)
	{
		setReplacedEntry(true);
	}

	if (isCompressed())
	{
		setUncompressedSize(uiUncompressedSize);

		switch (getCompressionAlgorithmId())
		{
		case COMPRESSION_ZLIB:
			strEntryData = CompressionManager::compressZLib(strEntryData, getCompressionLevel());
			break;
		case COMPRESSION_LZ4:
			strEntryData = CompressionManager::compressLZ4(strEntryData, getCompressionLevel());
			break;
		case COMPRESSION_LZO_1X_999:
			strEntryData = compressXBOXIMGEntry(strEntryData);
			break;
		}
	}

	strEntryData = String::zeroPad(strEntryData, IMGEntry::getEntryDataPadLength(strEntryData.length()));

	uint32
		uiPreviousEntrySize = m_uiEntrySize,
		uiNewEntrySize = (uint32)strEntryData.length(),
		uiPreviousEntrySizeSectors = Math::convertBytesToSectors(uiPreviousEntrySize),
		uiNewEntrySizeSectors = Math::convertBytesToSectors(uiNewEntrySize);

	setEntrySize(uiNewEntrySize);
	unserializeRWVersion(nullptr, "", strEntryData);

	if (uiNewEntrySizeSectors <= uiPreviousEntrySizeSectors)
	{
		uint32 uiEntrySizePadded = 2048 * uiPreviousEntrySizeSectors;
		strEntryData = String::zeroPad(strEntryData, uiEntrySizePadded);
		File::storeFileSubContent(m_pIMGFile->getIMGFilePath(), strEntryData, getEntryOffset());
	}
	else
	{
		uint32 uiNewEntryOffset = m_pIMGFile->getNextEntryOffset();
		setEntryOffset(uiNewEntryOffset);
		File::storeFileSubContent(m_pIMGFile->getIMGFilePath(), strEntryData, uiNewEntryOffset);
	}
}

string					IMGEntry::getEntryData(void)
{
	string strEntryData = File::getFileSubContent(getIMGFile()->getIMGFilePath(), getEntryOffset(), getEntrySize(), true);

	if (isCompressed())
	{
		// compressed
		switch (getCompressionAlgorithmId())
		{
		case COMPRESSION_ZLIB:			return CompressionManager::decompressZLib(strEntryData, getUncompressedSize());
		case COMPRESSION_LZ4:			return CompressionManager::decompressLZ4(strEntryData, getUncompressedSize());
		case COMPRESSION_LZO_1X_999:	return decompressXBOXIMGEntry(strEntryData);
		default:						return "";
		}
	}
	else
	{
		// not compressed
		return strEntryData;
	}
}

string					IMGEntry::getEntrySubData(uint32 uiStart, uint32 uiLength)
{
	string strIMGFilePath = getIMGFile()->getIMGFilePath();

	if (isCompressed())
	{
		// compressed
		const uint8 ucZLibBlockSize = 16;
		uint32 uiStart2 = (uint32) floor((float32)uiStart / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiLength2 = (uint32) ceil((float32)uiLength / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiStartOffset = uiStart - uiStart2;

		string strEntrySubData = File::getFileSubContent(strIMGFilePath, getEntryOffset() + uiStart2, uiLength2, true);
		switch (getCompressionAlgorithmId())
		{
		case COMPRESSION_ZLIB:			return CompressionManager::decompressZLib(strEntrySubData, getUncompressedSize());
		case COMPRESSION_LZ4:			return CompressionManager::decompressLZ4(strEntrySubData, getUncompressedSize());
		case COMPRESSION_LZO_1X_999:	return decompressXBOXIMGEntry(strEntrySubData);
		}
		return "";
	}
	else
	{
		// not compressed
		return File::getFileSubContent(strIMGFilePath, getEntryOffset() + uiStart, uiLength, true);
	}
}

string					IMGEntry::compressXBOXIMGEntry(string& strEntryData)
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

	return addXBOXCompressionMetaData(strOutputData);
}

string					IMGEntry::decompressXBOXIMGEntry(string& strEntryData)
{
	// the entry data is compressed with LZO 1X 999 compression
	bool bBigEndian = false;
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

string					IMGEntry::addXBOXCompressionMetaData(string& strCompressedEntryData)
{
	bool bBigEndian = false;

	uint32 uiMagicNumber = 0x67A3A1CE;
	uint32 uiChecksum = 0; // todo
	uint32 uiCompressionDataSize = strCompressedEntryData.length();

	string strEntryMetaData = "";
	strEntryMetaData += String::packUint32(uiMagicNumber, bBigEndian);
	strEntryMetaData += String::packUint32(uiChecksum, bBigEndian);
	strEntryMetaData += String::packUint32(uiCompressionDataSize, bBigEndian);

	return strEntryMetaData + strCompressedEntryData;
}

void					IMGEntry::saveEntry(string strFilePath)
{
	File::storeFile(strFilePath, getEntryData(), false, true);
}

void					IMGEntry::saveEntryByMemory(string strFilePath, string& strEntryData)
{
	File::storeFile(strFilePath, strEntryData, false, true);
}

uint32					IMGEntry::getEntryDataPadLength(uint32 uiUnpaddedDataLength)
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

uint32					IMGEntry::getVersion3IMGSizeDeduction(uint32 uiDataLength)
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

bool					IMGEntry::canBeRead(void)
{
	// container encryption
	if (getIMGFile()->isEncrypted() && getIMGFile()->getEncryptionType() == ENCRYPTION_UNKNOWN)
	{
		return false;
	}

	// container encryption
	if (isEncrypted() && getEncryptionAlgorithmId() == ENCRYPTION_UNKNOWN)
	{
		return false;
	}

	// container compression
	if (isCompressed() && getCompressionAlgorithmId() == COMPRESSION_UNKNOWN)
	{
		return false;
	}

	return true;
}

string					IMGEntry::getVersionText(void)
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

void					IMGEntry::replace(string& strFilePath)
{
	string strFileName = Path::getFileName(strFilePath);

	string strFileData = File::getFileContent(strFilePath);

	setEntryName(strFileName);
	setEntrySize(File::getFileSize(strFilePath));
	setEntryOffset(m_pIMGFile->getNextEntryOffset());
	if (m_pIMGFile->getVersion() == IMG_3)
	{
		// todo
	}
	else
	{
		if (strFileData.length() >= 12)
		{
			unserializeRWVersion(nullptr, "", strFileData.substr(0, 12));
		}
		else
		{
			setRawVersion(0);
		}
	}
}

void					bxgi::IMGEntry::unserializeVersion1(bxgi::RG_IMGEntry_Version1 *pRGIMGEntry)
{
	m_uiEntryOffset = bxcf::Math::convertSectorsToBytes(pRGIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = bxcf::Math::convertSectorsToBytes(pRGIMGEntry->m_uiSizeInSectors);
	m_strEntryName = bxcf::String::rtrimFromLeft(std::string((char*)pRGIMGEntry->m_szName));
}

void					bxgi::IMGEntry::unserializeVersion2(bxgi::RG_IMGEntry_Version2 *pRGIMGEntry)
{
	uint32 uiEntrySizeInSectors;
	if (pRGIMGEntry->m_uiArchiveSizeInSectors != 0)
	{
		uiEntrySizeInSectors = pRGIMGEntry->m_uiArchiveSizeInSectors;
	}
	else
	{
		uiEntrySizeInSectors = pRGIMGEntry->m_uiStreamingSizeInSectors;
	}

	m_uiEntryOffset = bxcf::Math::convertSectorsToBytes(pRGIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = bxcf::Math::convertSectorsToBytes(uiEntrySizeInSectors);
	m_strEntryName = bxcf::String::rtrimFromLeft(std::string((char*)pRGIMGEntry->m_szName));
}

void					bxgi::IMGEntry::unserializeVersion3(bxgi::RG_IMGEntry_Version3 *pRGIMGEntry)
{
	setRageResourceTypeByIdentifier(pRGIMGEntry->m_uiRageResourceTypeIdentifier);
	m_uiEntryOffset = bxcf::Math::convertSectorsToBytes(pRGIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = bxcf::Math::convertSectorsToBytes(pRGIMGEntry->m_usSizeInSectors) - (pRGIMGEntry->m_usFlags & 2047);
	m_uiFlags = pRGIMGEntry->m_usFlags;
}

void					bxgi::IMGEntry::unserializeVersionFastman92(bxgi::IMGEntry_Fastman92 *pRawIMGEntry)
{
	m_uiEntryOffset = bxcf::Math::convertSectorsToBytes(pRawIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = bxcf::Math::convertSectorsToBytes(pRawIMGEntry->m_uiSizeInSectors);
	m_uiUncompressedSize = bxcf::Math::convertSectorsToBytes(pRawIMGEntry->m_uiUncompressedSizeInSectors);
	m_uiCompressionAlgorithm = bxgi::IMGFormat::getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId((bxgi::EIMGVersionFastman92CompressionAlgorithm)pRawIMGEntry->m_uiCompressionAlgorithmId);
	m_strEntryName = bxcf::String::rtrimFromLeft(std::string((char*)pRawIMGEntry->m_strName));
}