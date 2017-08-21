#include "CIMGEntry.h"
#include "CIMGFormat.h"
#include "CIMGManager.h"
#include "Static/CPath.h"
#include "Static/CString2.h"
#include "Static/CFile.h"
#include "Static/CDebug.h"
#include "Compression/CCompressionManager.h"
#include "Localization/CLocalizationManager.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Engine/RAGE/CRageManager.h"
#include "Engine/RAGE/CRageResourceTypeManager.h"
#include "Format/IMG/Regular/Raw/CIMGEntry_Version1Or2.h"
#include "Format/IMG/Regular/Raw/CIMGEntry_Version3.h"
#include "Format/IMG/Fastman92/CIMGEntry_Fastman92.h"
#include "Format/COL/CCOLManager.h"
#include "Format/COL/CCOLVersionManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;
using namespace bxcf::fileType;

CIMGEntry::CIMGEntry(void) :
	m_pIMGFile(nullptr),
	m_uiEntryOffset(0),
	m_uiEntrySize(0),
	m_bNewEntry(false),
	m_bReplacedEntry(false),
	m_bProtectedEntry(false),
	m_uiFileCreationDate(0),
	m_eCompressionAlgorithm(COMPRESSION_NONE),
	m_uiCompressionLevel(0),
	m_bIsEncrypted(false),

	m_pRageResourceType(nullptr),
	
	m_pRWVersion(nullptr),
	m_pCOLVersion(nullptr),
	m_uiUncompressedSize(0),
	m_uiFlags(0),
	m_uiRawVersion(0)
{
}

CIMGEntry::CIMGEntry(CIMGFormat *pIMGFile) :
	m_pIMGFile(pIMGFile),
	m_uiEntryOffset(0),
	m_uiEntrySize(0),
	m_bNewEntry(false),
	m_bReplacedEntry(false),
	m_bProtectedEntry(false),
	m_uiFileCreationDate(0),
	m_eCompressionAlgorithm(COMPRESSION_NONE),
	m_uiCompressionLevel(0),
	m_bIsEncrypted(false),

	m_pRageResourceType(nullptr),

	m_pRWVersion(nullptr),
	m_pCOLVersion(nullptr),
	m_uiUncompressedSize(0),
	m_uiFlags(0),
	m_uiFileType(bxcf::fileType::UNKNOWN)
{
}

void					CIMGEntry::setEntrySize(uint32 uiEntrySize)
{
	m_uiEntrySize = uiEntrySize;
	if (getIMGFile()->getVersion() == IMG_3)
	{
		m_uiFlags = (m_uiFlags & 63488) | (uint16)getVersion3IMGSizeDeduction(uiEntrySize);
	}
}

uint32			CIMGEntry::getPaddedEntrySize(void)
{
	return (uint32) (ceil(((float32) getEntrySize()) / 2048.0f) * 2048.0f);
}

void					CIMGEntry::setEntryData(string strEntryData, bool bIsNew)
{
	//setEntryOffset(getIMGFile()->getNextEntryOffset()); // todo - this line is needed but getNextEntryOffset isnt defined yet
	uint32 uiFUncompressedSize = (uint32)strEntryData.length();
	if(!bIsNew)
	{
		setReplacedEntry(true);
	}

	strEntryData = CString2::zeroPad(strEntryData, CIMGEntry::getEntryDataPadLength(strEntryData.length()));
	if (getIMGFile()->getVersion() == IMG_FASTMAN92)
	{
		setUncompressedSize(uiFUncompressedSize);

		if (isCompressed())
		{
			// compressed
			switch (getCompressionAlgorithmId())
			{
			case COMPRESSION_ZLIB:
				strEntryData = CCompressionManager::compressZLib(strEntryData, getCompressionLevel());
				break;
			case COMPRESSION_LZ4:
				strEntryData = CCompressionManager::compressLZ4(strEntryData, getCompressionLevel());
				break;
			}
		}
		else
		{
			// not compressed
		}
	}
	/*
	// todo - add getPlatform() to CIMGFormat and uncomment this code
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
				strCompressedData = CCompressionManager::compressLZO1X999(strEntryData.substr(i * uiBlockSize));
			}
			else
			{
				strCompressedData = CCompressionManager::compressLZO1X999(strEntryData.substr(i * uiBlockSize, uiBlockSize));
			}

			uint32 uiUnknown1 = 4;
			uint32 uiUncompressedBlockSize = strEntryData.length();
			uint32 uiCompressedBlockSize = strCompressedData.length();

			strOutputData += CString2::packUint32(uiUnknown1, bBigEndian);
			strOutputData += CString2::packUint32(uiUncompressedBlockSize, bBigEndian);
			strOutputData += CString2::packUint32(uiCompressedBlockSize, bBigEndian);

			strOutputData += strCompressedData;
		}

		uint32 uiMagicNumber = 0x67A3A1CE;
		uint32 uiChecksum = 0;
		uint32 uiCompressionDataSize = strOutputData.length();

		string strOutputData2 = "";
		strOutputData2 += CString2::packUint32(uiMagicNumber, bBigEndian);
		strOutputData2 += CString2::packUint32(uiChecksum, bBigEndian);
		strOutputData2 += CString2::packUint32(uiCompressionDataSize, bBigEndian);
		
		strEntryData = strOutputData2 + strOutputData;
	}
	*/
	setEntrySize((uint32)strEntryData.length());
	CFile::storeFileSubContent(getIMGFile()->getFilePath(), strEntryData, getEntryOffset());
}

string					CIMGEntry::getEntryData(void)
{
	string strEntryData = CFile::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset(), getEntrySize(), true);
	bool bBigEndian = false;

	if (getIMGFile()->getVersion() == IMG_FASTMAN92)
	{
		if (isCompressed())
		{
			// compressed
			switch (getCompressionAlgorithmId())
			{
			case COMPRESSION_ZLIB:	return CCompressionManager::decompressZLib(strEntryData, getUncompressedSize());
			case COMPRESSION_LZ4:	return CCompressionManager::decompressLZ4(strEntryData, getUncompressedSize());
			default:					return "";
			}
		}
		else
		{
			// not compressed
			return strEntryData;
		}
	}
	else if (CString2::unpackUint32(strEntryData.substr(0, 4), bBigEndian) == 0x67A3A1CE)
	{
		// the entry data is compressed with LZO 1X 999 compression
		uint32 uiSeek = 0;

		uint32 uiMagicNumber = CString2::unpackUint32(strEntryData.substr(uiSeek, 4), bBigEndian);
		uint32 uiChecksum = CString2::unpackUint32(strEntryData.substr(uiSeek + 4, 4), bBigEndian);
		uint32 uiCompressionDataSize = CString2::unpackUint32(strEntryData.substr(uiSeek + 8, 4), bBigEndian);
		uiSeek += 12;

		string strUncompressedData = "";
		uint32 uiByteCountRead = 0;
		while (uiByteCountRead < uiCompressionDataSize)
		{
			uint32 uiUnknown1 = CString2::unpackUint32(strEntryData.substr(uiSeek, 4), bBigEndian);
			uint32 uiUncompressedBlockSize = CString2::unpackUint32(strEntryData.substr(uiSeek + 4, 4), bBigEndian);
			uint32 uiCompressedBlockSize = CString2::unpackUint32(strEntryData.substr(uiSeek + 8, 4), bBigEndian);

			string strCompressedBlock = strEntryData.substr(uiSeek + 12, uiCompressedBlockSize);
			strUncompressedData += CCompressionManager::decompressLZO1X(strCompressedBlock, uiUncompressedBlockSize);

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

string					CIMGEntry::getEntrySubData(uint32 uiStart, uint32 uiLength)
{
	if (getIMGFile()->getVersion() == IMG_FASTMAN92 && isCompressed())
	{
		// compressed
		const uint8 ucZLibBlockSize = 16;
		uint32 uiStart2 = (uint32) floor((float32)uiStart / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiLength2 = (uint32) ceil((float32)uiLength / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiStartOffset = uiStart - uiStart2;
		/*
		CDebug::log("uiStart: " + CString2::toString(uiStart));
		CDebug::log("uiLength: " + CString2::toString(uiLength));
		CDebug::log("uiStart2: " + CString2::toString(uiStart2));
		CDebug::log("uiLength2: " + CString2::toString(uiLength2));
		CDebug::log("uiStartOffset: " + CString2::toString(uiStartOffset));
		*/
		string strEntrySubData = CFile::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset() + uiStart2, uiLength2, true);
		//strEntrySubData = CIMGManager::decompressZLib(strEntrySubData, uiLength2);
		switch (getCompressionAlgorithmId())
		{
		case COMPRESSION_ZLIB:	return CCompressionManager::decompressZLib(strEntrySubData, getUncompressedSize());
		case COMPRESSION_LZ4:	return CCompressionManager::decompressLZ4(strEntrySubData, getUncompressedSize());
		}
		return strEntrySubData.substr(uiStartOffset, uiLength);
	}
	else
	{
		// not compressed
		return CFile::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset() + uiStart, uiLength, true);
	}
}

void					CIMGEntry::saveEntry(string strFilePath)
{
	CFile::storeFile(strFilePath, getEntryData(), false, true);
}

void					CIMGEntry::saveEntryByMemory(string strFilePath, string& strEntryData)
{
	CFile::storeFile(strFilePath, strEntryData, false, true);
}

uint32			CIMGEntry::getEntryDataPadLength(uint32 uiUnpaddedDataLength)
{
	return (uint32)(ceil((float32)uiUnpaddedDataLength / (float32)2048.0f) * (float32)2048.0f);
}

void					CIMGEntry::setRWVersionByVersionCC(uint32 uiRWVersionCC)
{
	setRWVersion(CRWManager::get()->getVersionManager()->getEntryByVersionCC(uiRWVersionCC));
}

void					CIMGEntry::setRageResourceTypeByIdentifier(uint32 uiResourceType)
{
	setRageResourceType(CRageManager::get()->getResourceTypeManager()->getResourceTypeByIdentifier(uiResourceType));
}

uint32			CIMGEntry::getVersion3IMGSizeDeduction(uint32 uiDataLength)
{
	return 2048 - (uiDataLength % 2048);
}

bool					CIMGEntry::isRWFile(void)
{
	return m_uiFileType == TEXTURE || m_uiFileType == MODEL;
}

bool					CIMGEntry::isModelFile(void)
{
	return m_uiFileType == MODEL;
}

bool					CIMGEntry::isTextureFile(void)
{
	return m_uiFileType == TEXTURE;
}

bool					CIMGEntry::isCollisionFile(void)
{
	return m_uiFileType == COLLISION;
}

/*
string					CIMGEntry::getEntryDecompressedData(void)
{

}
string					CIMGEntry::getEntryCompressedData(void)
{

}
*/

void					CIMGEntry::applyCompression(eCompressionAlgorithm eCompressionAlgorithmValue, uint32 uiCompressionLevel)
{
	string strEntryData = getEntryData();
	setCompressionAlgorithmId(eCompressionAlgorithmValue);
	setCompressionLevel(uiCompressionLevel);
	setEntryData(strEntryData);
}

bool					CIMGEntry::doesHaveUnknownVersion(void)
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

CIMGEntry*				CIMGEntry::clone(CIMGFormat *pIMGFile)
{
	CIMGEntry *pClonedIMGEntry = new CIMGEntry(pIMGFile);

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

bool				CIMGEntry::canBeRead(void)
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

std::string				CIMGEntry::getVersionText(void)
{
	if (getIMGFile()->getVersion() == IMG_3)
	{
		// IMG version 3
		if (getRageResourceType() == nullptr)
		{
			return CLocalizationManager::get()->getTranslatedText("Window_Main_Combo_RWVersion_Unknown");
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
			return CRWManager::get()->getVersionManager()->getVersionText(m_uiRawVersion);
		case COLLISION:
			// COL file
			return CCOLManager::get()->getVersionManager()->getVersionText(m_uiRawVersion);
		case ANIMATION:
			// IFP file
			return "IFP " + CString2::toString(m_uiRawVersion);
		}
	}
	return "Unknown";
}