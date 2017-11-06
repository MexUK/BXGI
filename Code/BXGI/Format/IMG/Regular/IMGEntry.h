#pragma once

#include "nsbxgi.h"
#include "Compression/ECompressionAlgorithm.h"
#include "Encryption/EEncryptionAlgorithm.h"
#include "Format/EFileType.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version1.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version2.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version3.h"
#include "Format/IMG/Fastman92/IMGEntry_Fastman92.h"
#include "Static/Math.h"
#include "Static/String.h"
//#include "Format/IMG/Regular/IMGFormat.h"
#include "Game/EGame.h"
#include "Format/FormatEntry.h"
#include <string>

class bxgi::RWVersion;
class bxgi::COLVersion;
class bxgi::RageResourceType;

class bxgi::IMGEntry : public bxcf::FormatEntry
{
public:
	IMGEntry(void);
	IMGEntry(bxgi::IMGFormat *pIMGFile);

	void					unload(void) {}

	void					unserializeVersion1(bxgi::RG_IMGEntry_Version1 *pRGIMGEntry);
	void					unserializeVersion2(bxgi::RG_IMGEntry_Version2 *pRGIMGEntry);
	void					unserializeVersion3(bxgi::RG_IMGEntry_Version3 *pRGIMGEntry);
	void					unserializeVersionFastman92(bxgi::IMGEntry_Fastman92 *pRawIMGEntry);

	void					unserializeRWVersion(bxcf::DataReader *pDataReader = nullptr, std::string strFilePath = "", std::string& strUncompressedEntryData = bxcf::g_strBlankString);
	void					unserializeResourceType(bxcf::DataReader *pDataReader = nullptr);

	void					replace(std::string& strFilePath);

	uint32					getIndex(void);
	inline std::string		getEntryExtension(void) { return m_strEntryExtension; }
	inline uint32			getEntryOffset(void) { return m_uiEntryOffset; } // in bytes
	inline uint32			getEntrySize(void) { return m_uiEntrySize; } // in bytes
	std::string				getVersionText(void);

	void					setIMGFile(bxgi::IMGFormat *pIMGFile) { m_pIMGFile = pIMGFile; }
	bxgi::IMGFormat*		getIMGFile(void) { return m_pIMGFile; }

	void					setEntryOffset(uint32 uiEntryOffsetInBytes) { m_uiEntryOffset = uiEntryOffsetInBytes; } // in bytes
	void					setEntryOffsetInSectors(uint32 uiEntryOffsetInSectors) { m_uiEntryOffset = bxcf::Math::convertSectorsToBytes(uiEntryOffsetInSectors); } // in sectors (1 sector = 2048 bytes)
	inline uint32			getEntryOffsetInSectors(void) { return bxcf::Math::convertBytesToSectors(m_uiEntryOffset); } // in sectors (1 sector = 2048 bytes)
	uint32					getUndecryptedEntryOffset(void);

	void					setEntrySize(uint32 uiEntrySize); // in bytes
	uint32					getPaddedEntrySize(void); // in bytes
	void					setEntrySizeInSectors(uint32 uiEntrySizeInSectors) { m_uiEntrySize = bxcf::Math::convertSectorsToBytes(uiEntrySizeInSectors); } // in sectors (1 sector = 2048 bytes)
	inline uint32			getEntrySizeInSectors(void) { return bxcf::Math::convertBytesToSectors(getPaddedEntrySize()); } // in sectors (1 sector = 2048 bytes)

	void					setEntryName(std::string& strEntryName) { m_strEntryName = strEntryName; } // dynamic length
	inline std::string&		getEntryName(void) { return m_strEntryName; }  // dynamic length

	inline void				setEntryExtension(std::string& strEntryExtension) { m_strEntryExtension = strEntryExtension; }

	void					setRageResourceTypeByIdentifier(uint32 uiResourceType);
	void					setRageResourceType(bxgi::RageResourceType *pRageResourceType) { m_pRageResourceType = pRageResourceType; }
	bxgi::RageResourceType*	getRageResourceType(void) { return m_pRageResourceType; }

	void					setFlags(uint32 usFlags) { m_uiFlags = usFlags; }
	uint32					getFlags(void) { return m_uiFlags; }

	void							setUncompressedSize(uint32 uiUncompressedSize) { m_uiUncompressedSize = uiUncompressedSize; } // in bytes
	uint32							getUncompressedSize(void) { return m_uiUncompressedSize; } // in bytes
	void							setUncompressedSizeInSectors(uint32 uiUncompressedSizeInSectors) { m_uiUncompressedSize = bxcf::Math::convertSectorsToBytes(uiUncompressedSizeInSectors); } // in sectors (1 sector = 2048 bytes)
	uint32							getUncompressedSizeInSectors(void) { return bxcf::Math::convertBytesToSectors(m_uiUncompressedSize); } // in sectors (1 sector = 2048 bytes)

	inline bool						isCompressed(void) { return m_uiCompressionAlgorithm != bxcf::COMPRESSION_UNKNOWN && m_uiCompressionAlgorithm != bxcf::COMPRESSION_NONE; }

	void							setCompressionAlgorithmId(bxcf::ECompressionAlgorithm ECompressionAlgorithmValue) { m_uiCompressionAlgorithm = ECompressionAlgorithmValue; }
	bxcf::ECompressionAlgorithm		getCompressionAlgorithmId(void) { return m_uiCompressionAlgorithm; }

	void							setCompressionLevel(uint32 uiCompressionLevel) { m_uiCompressionLevel = uiCompressionLevel; }
	uint32							getCompressionLevel(void) { return m_uiCompressionLevel; }

	void							setEncrypted(bool bIsEncrypted) { m_bIsEncrypted = bIsEncrypted; }
	bool							isEncrypted(void) { return m_bIsEncrypted; }

	void							setEncryptionAlgorithmId(bxcf::EEncryptionAlgorithm ECompressionAlgorithmValue) { m_uiEncryptionAlgorithm = ECompressionAlgorithmValue; }
	bxcf::EEncryptionAlgorithm		getEncryptionAlgorithmId(void) { return m_uiEncryptionAlgorithm; }

	void					setNewEntry(bool bNewEntry) { m_bNewEntry = bNewEntry; }
	bool					isNewEntry(void) { return m_bNewEntry; }

	void					setReplacedEntry(bool bReplacedEntry) { m_bReplacedEntry = bReplacedEntry; }
	bool					isReplacedEntry(void) { return m_bReplacedEntry; }

	void					setProtectedEntry(bool bProtectedEntry) { m_bProtectedEntry = bProtectedEntry; }
	bool					isProtectedEntry(void) { return m_bProtectedEntry; }

	void					setFileCreationDate(uint32 uiFileCreationDate) { m_uiFileCreationDate = uiFileCreationDate; }
	uint32					getFileCreationDate(void) { return m_uiFileCreationDate; }

	inline void				setRawVersion(uint32 uiRawVersion) { m_uiRawVersion = uiRawVersion; }
	inline uint32			getRawVersion(void) { return m_uiRawVersion; }

	void					setCOLVersion(bxgi::ECOLVersion uiCOLVersion) { m_uiRawVersion = uiCOLVersion; }
	bxgi::ECOLVersion		getCOLVersion(void) { return (bxgi::ECOLVersion) m_uiRawVersion; }

	void					setRWVersionByVersionCC(uint32 uiRWVersionCC);
	void					setRWVersion(uint32 uiRWVersion) { m_uiRawVersion = uiRWVersion; } // todo
	uint32					getRWVersion(void) { return m_uiRawVersion; } // todo

	void					setEntryData(std::string& strEntryData, bool bIsNew = false);	// takes uncmpressed data, and stores it compressed if the entry is marked as compressed
	std::string				getEntryData(void);												// returns uncompressed data if the entry is marked as compressed
	std::string				getEntrySubData(uint32 uiStart, uint32 uiLength);

	std::string				compressXBOXIMGEntry(std::string& strEntryData);
	std::string				decompressXBOXIMGEntry(std::string& strEntryData);
	std::string				addXBOXCompressionMetaData(std::string& strCompressedEntryData);

	bool					canBeRead(void);

	void					saveEntry(std::string strFilePath);
	void					saveEntryByMemory(std::string strFilePath, std::string& strEntryData);

	static uint32			getEntryDataPadLength(uint32 uiUnpaddedDataLength);
	static uint32			getVersion3IMGSizeDeduction(uint32 uiDataLength);

	bool					isRWFile(void);
	bool					isModelFile(void);
	bool					isTextureFile(void);
	bool					isCollisionFile(void);
	
	inline void							setFileType(bxcf::fileType::EFileType uiFileType) { m_uiFileType = uiFileType; }
	inline bxcf::fileType::EFileType	getFileType(void) { return m_uiFileType; }

	void					applyCompression(bxcf::ECompressionAlgorithm ECompressionAlgorithmValue, uint32 uiCompressionLevel = 0);

	bool					doesHaveUnknownVersion(void); // checks for RW version (DFF/TXD) or COL version. Unknown file extension counts as unknown RW version. IPL files are skipped.

	bxgi::IMGEntry*			clone(bxgi::IMGFormat *pIMGFile);

private:
	bxgi::IMGFormat*		m_pIMGFile;
	uint32					m_uiEntryOffset; // in bytes
	uint32					m_uiEntrySize; // in bytes
	std::string				m_strEntryName;
	std::string				m_strEntryExtension;
	uint32					m_uiFlags;
	union {
		struct { // IMG version 3 (encrypted / unencrypted)
			bxgi::RageResourceType*	m_pRageResourceType;
		};
		struct { // IMG version fastman92
			uint32						m_uiUncompressedSize; // in bytes
		};
	};
	uint8							m_bNewEntry					: 1;
	uint8							m_bReplacedEntry			: 1;
	uint8							m_bProtectedEntry			: 1;
	uint8							m_bIsEncrypted				: 1;
	uint32							m_uiFileCreationDate;
	bxcf::ECompressionAlgorithm		m_uiCompressionAlgorithm;
	bxcf::EEncryptionAlgorithm		m_uiEncryptionAlgorithm;
	uint32							m_uiCompressionLevel;
	uint32							m_uiRawVersion;
	bxcf::fileType::EFileType		m_uiFileType;
};