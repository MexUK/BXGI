#include "IMGFormat.h"
#include "nsbxgi.h"
#include "Type/Types.h"
#include "Event/Events.h"
#include "Event/EEvent.h"
#include "Stream/DataWriter.h"
#include "IMGManager.h"
#include "Static/String.h"
#include "Format/IMG/Regular/Raw/IMGFormat_Version2_Header1.h"
#include "Format/IMG/Regular/Raw/IMGFormat_Version3_Header1.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version1Or2.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version3.h"
#include "Exception/EExceptionCode.h"
#include "Static/Path.h"
#include "Format/COL/COLManager.h"
#include "Localization/LocalizationManager.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersion.h"
#include "Static/Memory.h"
#include "Format/IMG/Fastman92/IMGFormat_VersionFastman92_Header1.h"
#include "Format/IMG/Fastman92/IMGFormat_VersionFastman92_Header2.h"
#include "Format/IMG/Fastman92/IMGEntry_Fastman92.h"
#include "Engine/RAGE/RageManager.h"
#include "Engine/RAGE/RageResourceTypeManager.h"
#include "Static/File.h"
#include "Static/Timing.h"
#include "Static/Debug.h"
#include "Format/EFileType.h"
#include "Static/StdVector.h"
#include "IMGEntry.h"
#include "../BXCF/Event/EEvent.h"
#include "Format/DFF/DFFFormat.h"
#include <set>
#include <algorithm>

using namespace std;
using namespace bxcf;
using namespace bxcf::fileType;
using namespace bxgi;

IMGFormat::IMGFormat(void) :
	Format(true, LITTLE_ENDIAN),
	m_EIMGVersion(IMG_UNKNOWN),
	m_EPlatform(PLATFORM_PC),
	m_bEncrypted(false),
	m_ucGameType(0)
{
}

IMGFormat::IMGFormat(std::string& strFilePathOrData, bool bStringIsFilePath) :
	Format(strFilePathOrData, bStringIsFilePath, true, LITTLE_ENDIAN),
	m_EIMGVersion(IMG_UNKNOWN),
	m_EPlatform(PLATFORM_PC),
	m_bEncrypted(false),
	m_ucGameType(0)
{
}

IMGFormat::IMGFormat(DataReader& reader) :
	Format(reader, true, LITTLE_ENDIAN),
	m_EIMGVersion(IMG_UNKNOWN),
	m_EPlatform(PLATFORM_PC),
	m_bEncrypted(false),
	m_ucGameType(0)
{
}

// meta data
void				IMGFormat::readMetaData(void)
{
	if (String::toUpperCase(Path::getFileExtension(m_strFilePath)) == "DIR")
	{
		// version 1
		m_EIMGVersion = IMG_1;
		m_uiEntryCount = (uint32)(m_reader.getSize() / 32);
		return;
	}

	string
		strFirst16Bytes = m_reader.read(16),
		strFirst4Bytes = strFirst16Bytes.substr(0, 4);
	uint32
		uiSecond4BytesUi = String::unpackUint32(strFirst16Bytes.substr(4, 4), false);

	if (strFirst4Bytes == "VER2")
	{
		// version 2
		m_EIMGVersion = IMG_2;
		m_uiEntryCount = uiSecond4BytesUi;
		return;
	}

	if (strFirst4Bytes == "VERF")
	{
		// version fastman92
		m_EIMGVersion = IMG_FASTMAN92;
		m_uiEntryCount = uiSecond4BytesUi;
		m_ucGameType = String::unpackUint8(strFirst16Bytes.substr(8, 1));
		return;
	}

	uint32 uiIdentifier = String::unpackUint32(strFirst4Bytes, false);
	bool bEncrypted = false;

	if (uiIdentifier != 0xA94E2A52)
	{
		bEncrypted = true;
		strFirst16Bytes = IMGManager::decryptVersion3IMGString(strFirst16Bytes);
		uiIdentifier = String::unpackUint32(strFirst16Bytes.substr(0, 4), false);
		uiSecond4BytesUi = String::unpackUint32(strFirst16Bytes.substr(4, 4), false);
	}

	if (uiIdentifier == 0xA94E2A52)
	{
		if (uiSecond4BytesUi == 3)
		{
			// version 3
			m_EIMGVersion = IMG_3;
			m_uiEntryCount = String::unpackUint32(strFirst16Bytes.substr(8, 4), false);
			m_bEncrypted = bEncrypted;
			return;
		}
	}

	string strFilePathDIRExt = getDIRFilePath();
	if (File::doesFileExist(strFilePathDIRExt))
	{
		m_reader.reset();
		m_reader.setFilePath(strFilePathDIRExt);
		m_strFilePath = strFilePathDIRExt; // temp
		if (!m_reader.open(m_bFormatUsesBinaryData))
		{
			//throw new FAILED_TO_OPEN_FILE;
			return;
		}

		// version 1
		m_EIMGVersion = IMG_1;
		m_uiEntryCount = (uint32)(m_reader.getSize() / 32);
		return;
	}
}

// validation
bool				IMGFormat::validate(void)
{
	if (getVersion() == IMG_FASTMAN92)
	{
		// check if IMG is fastman92 format and is encrypted
		if (isEncrypted())
		{
			setErrorCode(EXCEPTION_UNKNOWN_ENCRYPTION_ALGORITHM_USED);
			return false;
		}

		// check if IMG is fastman92 format and has an unsupported game type
		if (getGameType() != 0)
		{
			setErrorCode(EXCEPTION_UNSUPPORTED_GAME_USED);
			return false;
		}
	}

	// no errors occurred
	return true;
}

// version
EIMGVersion			IMGFormat::getVersion(void)
{
	checkMetaDataIsLoaded();
	return m_EIMGVersion;
}

// unserialize
void				IMGFormat::_unserialize(void)
{
	checkMetaDataIsLoaded();

	switch (m_EIMGVersion)
	{
	case IMG_1:
		unserializeVersion1();
		break;
	case IMG_2:
		unserializeVersion2();
		break;
	case IMG_3:
		if (isEncrypted())
		{
			unserializeVersion3_Encrypted();
		}
		else
		{
			unserializeVersion3_Unencrypted();
		}
		break;
	case IMG_FASTMAN92:
		unserializeVersionFastman92();
		break;
	}

	if (m_EIMGVersion != IMG_3)
	{
		unserializERWVersions(); // todo - fix function name - capital e in name
	}
}

// read entry content, with try/catch
string		IMGFormat::readEntryContent(uint32 uiEntryIndex)
{
	string strEntryData;
	try
	{
		IMGEntry *pIMGEntry = getEntryByIndex(uiEntryIndex);
		m_reader.setSeek(pIMGEntry->getEntryOffset());
		strEntryData = m_reader.readString(pIMGEntry->getEntrySize());
	}
	catch (EExceptionCode uiErrorCode)
	{
	}
	return strEntryData;
}

// IMG/DIR file path
string		IMGFormat::getIMGFilePath(void)
{
	return Path::replaceFileExtensionWithCase(m_strFilePath, "IMG");
}

string		IMGFormat::getDIRFilePath(void)
{
	return Path::replaceFileExtensionWithCase(m_strFilePath, "DIR");
}












// unserialization & serialization
void		IMGFormat::serialize(void)
{
	serializeHeaderAndBodyComponents();
}

// unserialization
void		IMGFormat::unserializeHeaderComponents(void)
{
	//IMGPeekData imgPeekData = peekIMGData();
	//setVersion(imgPeekData.getVersion());
	//setEncrypted(imgPeekData.isEncrypted());
	switch (getVersion())
	{
	case IMG_1:
		//Timing::get()->start("unserializeVersion1");
		unserializeVersion1();
		//Timing::get()->stop();
		break;
	case IMG_2:
		unserializeVersion2();
		break;
	case IMG_3:
		if (isEncrypted())
		{
			unserializeVersion3_Encrypted();
		}
		else
		{
			unserializeVersion3_Unencrypted();
		}
		break;
	case IMG_FASTMAN92:
		unserializeVersionFastman92();
		break;
	}
	loadEntryExtensionCounts();
}

void		IMGFormat::unserializeBodyComponents(void)
{
	switch (getVersion())
	{
	case IMG_1:
	case IMG_2:
	case IMG_FASTMAN92:
		//Timing::get()->start("unserializERWVersions");
		unserializERWVersions();
		//Timing::get()->stop();
		break;
	case IMG_3:
		unserializeResourceTypes();
		break;
	}
}

// header unserialization
void		IMGFormat::unserializeVersion1(void)
{
	// verify file size & fetch entry count
	uint64 uiFileSize = m_reader.getDataLength();
	if ((uiFileSize % 32) != 0)
	{
		throw EXCEPTION_INVALID_DATA_SIZE_MULTIPLE;
	}

	// load data from file into RG structs
	RG_IMGEntry_Version1Or2
		*pRGIMGEntries = m_reader.readStructMultiple<RG_IMGEntry_Version1Or2>(m_uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy RG structs into wrapper structs - so that we can use std::string for strings in our structs rather than char arrays
	vector<IMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize((unsigned int)m_uiEntryCount);

	for (uint64 i = 0; i < m_uiEntryCount; i++)
	{
		IMGEntry *pIMGEntry = new IMGEntry;
		rvecIMGEntries[(unsigned int)i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion1Or2(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())));
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}

	// clean up
	delete[] pRGIMGEntries;
}

void		IMGFormat::unserializeVersion2(void)
{
	// read header 1
	RG_IMGFormat_Version2_Header1 *pHeader1 = m_reader.readStruct<RG_IMGFormat_Version2_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;

	// load data from file into RG structs
	RG_IMGEntry_Version1Or2
		*pRGIMGEntries = m_reader.readStructMultiple<RG_IMGEntry_Version1Or2>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy RG structs into wrapper structs
	vector<IMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);

	for (uint64 i = 0; i < uiEntryCount; i++)
	{
		IMGEntry *pIMGEntry = new IMGEntry;
		rvecIMGEntries[(unsigned int)i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion1Or2(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())));
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}

	// clean up
	delete pHeader1;
	delete[] pRGIMGEntries;
}

void		IMGFormat::unserializeVersion3_Encrypted(void)
{
	DataReader *pDataReader2 = DataReader::get(1); // for unencrypted img header and unencrypted img table

	// decrypt IMG header
	string strIMGHeaderEncrypted = m_reader.readString(32); // padded with 12 bytes at the end
	string strIMGHeaderUnencrypted = IMGManager::decryptVersion3IMGString(strIMGHeaderEncrypted); // padded with 12 bytes at the end too
	//EDataStreamType ePreviousStreamType = pDataReader->getStreamType();

	pDataReader2->setStreamType(DATA_STREAM_MEMORY);
	pDataReader2->setData(strIMGHeaderUnencrypted);
	pDataReader2->setSeek(0);

	// read header 1
	RG_IMGFormat_Version3_Header1 *pHeader1 = pDataReader2->readStruct<RG_IMGFormat_Version3_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;

	// decrypt IMG table
	uint32 uiRemainder = (pHeader1->m_uiTableSize % 16);
	uint32 uiTablesLength = pHeader1->m_uiTableSize - uiRemainder;
	
	//pDataReader->setStreamType(ePreviousStreamType);
	m_reader.setSeek(20);
	
	string strIMGTableEncrypted = m_reader.readString(uiTablesLength);
	string strIMGBodyPartial = m_reader.readString(uiRemainder);
	string strIMGTableUnencrypted = IMGManager::decryptVersion3IMGString(strIMGTableEncrypted) + strIMGBodyPartial;
	
	//pDataReader2->setStreamType(DATA_STREAM_MEMORY);
	pDataReader2->setData(strIMGTableUnencrypted);
	pDataReader2->setSeek(0);

	RG_IMGEntry_Version3
		*pRGIMGEntries = pDataReader2->readStructMultiple<RG_IMGEntry_Version3>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;
	
	// copy IMG tables RG structs into wrapper structs
	vector<IMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		IMGEntry *pIMGEntry = new IMGEntry;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion3(pRGIMGActiveEntry++);
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}
	
	// read IMG entry names
	//pDataReader->setStreamType(ePreviousStreamType);
	//pDataReader->setSeek();
	
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		rvecIMGEntries[i]->setEntryName(pDataReader2->readStringUntilZero());
		rvecIMGEntries[i]->setEntryExtension(String::toUpperCase(Path::getFileExtension(rvecIMGEntries[i]->getEntryName())));
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}
	
	// restore
	//pDataReader->setStreamType(ePreviousStreamType);
	pDataReader2->reset();
}

void		IMGFormat::unserializeVersion3_Unencrypted(void)
{
	// read header 1
	RG_IMGFormat_Version3_Header1 *pHeader1 = m_reader.readStruct<RG_IMGFormat_Version3_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;

	// load data from file into RG structs
	RG_IMGEntry_Version3
		*pRGIMGEntries = m_reader.readStructMultiple<RG_IMGEntry_Version3>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy IMG tables RG structs into wrapper structs
	vector<IMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		IMGEntry *pIMGEntry = new IMGEntry;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion3(pRGIMGActiveEntry++);
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}

	// read IMG entry names
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		rvecIMGEntries[i]->setEntryName(m_reader.readStringUntilZero());
		rvecIMGEntries[i]->setEntryExtension(String::toUpperCase(Path::getFileExtension(rvecIMGEntries[i]->getEntryName())));
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}

	// clean up
	delete pHeader1;
	delete[] pRGIMGEntries;
}

void		IMGFormat::unserializeVersionFastman92(void)
{
	// read header 1
	IMGFormat_VersionFastman92_Header1 *pHeader1 = m_reader.readStruct<IMGFormat_VersionFastman92_Header1>();
	setGameType(pHeader1->m_uiGameId);

	// verify header 1 data
	if (pHeader1->m_uiFastman92IMGVersion != 1)
	{
		delete pHeader1;
		throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
	}
	if (pHeader1->m_uiEncryptionAlgorithmId != 0)
	{
		delete pHeader1;
		setEncrypted(true);
		throw EXCEPTION_UNKNOWN_ENCRYPTION_ALGORITHM_USED;
	}
	if (pHeader1->m_uiGameId != 0)
	{
		delete pHeader1;
		throw EXCEPTION_UNSUPPORTED_GAME_USED;
	}

	// read header 2
	IMGFormat_VersionFastman92_Header2 *pHeader2 = m_reader.readStruct<IMGFormat_VersionFastman92_Header2>();
	uint32 uiEntryCount = pHeader2->m_uiEntryCount;

	// verify header 2 data
	if (pHeader2->m_uiCheck != 1)
	{
		delete pHeader1;
		delete pHeader2;
		throw EXCEPTION_FILE_INVALID;
	}

	// load data from file into raw structs
	IMGEntry_Fastman92
		*pRawIMGEntries = m_reader.readStructMultiple<IMGEntry_Fastman92>(uiEntryCount),
		*pRawIMGActiveEntry = pRawIMGEntries;

	// copy raw structs into wrapper structs
	vector<IMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);

	for (uint64 i = 0, j = uiEntryCount; i < j; i++)
	{
		IMGEntry *pIMGEntry = new IMGEntry;
		rvecIMGEntries[(unsigned int)i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersionFastman92(pRawIMGActiveEntry++);
		pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())));
		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}

	// clean up
	delete pHeader1;
	delete pHeader2;
	delete[] pRawIMGEntries;
}

// body unserialization
void		IMGFormat::unserializERWVersions(void)
{
	bool bUseNewReader = getVersion() == IMG_1 && m_reader.getStreamType() == DATA_STREAM_FILE;
	DataReader *pDataReader = bUseNewReader ? new DataReader : &m_reader;

	if (bUseNewReader) // todo - IMG_1 and data stream memory
	{
		//pDataReader->close(); // close handle to DIR file

		pDataReader->setStreamType(DATA_STREAM_FILE);
		pDataReader->setFilePath(getIMGFilePath());
		pDataReader->open(true); // open handle to IMG file (handle is closed in Format::unserializeVia*() methods
	}

	for (IMGEntry *pIMGEntry : getEntries())
	{
		pIMGEntry->unserializeRWVersion(pDataReader);

		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}

	if (bUseNewReader) // todo - IMG_1 and data stream memory
	{
		pDataReader->close();
		delete pDataReader;
	}
}
void		IMGFormat::unserializeResourceTypes(void)
{
	// todo
}

// header & body serialization
void		IMGFormat::serializeHeaderAndBodyComponents(void)
{
	switch (getVersion())
	{
	case IMG_1:
		serializeVersion1();
		break;
	case IMG_2:
		serializeVersion2();
		break;
	case IMG_3:
		if (isEncrypted())
		{
			serializeVersion3_Encrypted();
		}
		else
		{
			serializeVersion3_Unencrypted();
		}
		break;
	case IMG_FASTMAN92:
		serializeVersionFastman92();
		break;
	}
}
void					IMGFormat::serializeVersion1(void)
{
	DataReader *pDataReader = DataReader::get();
	DataWriter *pDataWriter = DataWriter::get();

	string
		strDIRFilePathIn = getOriginalFilePath(),
		strIMGFilePathIn = Path::replaceFileExtensionWithCase(strDIRFilePathIn, "IMG"),
		strIMGFilePathOut = pDataWriter->getFilePath(),
		strDIRFilePathOut = Path::replaceFileExtensionWithCase(strIMGFilePathOut, "DIR");

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->setFilePath(strIMGFilePathIn);
		pDataReader->open(true);
	}

	// write IMG data
	uint32 uiSeek = 0;
	for (auto pIMGEntry : getEntries())
	{
		uint32 uiEntryByteCountPadded = IMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize());

		pDataReader->setSeek(pIMGEntry->getEntryOffset());
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()), uiEntryByteCountPadded);

		pIMGEntry->setEntryOffsetInSectors(uiSeek);
		uiSeek += uiEntryByteCountPadded;

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		// finalize IMG data reading
		pDataReader->close();
	}
	if (pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		// finalize IMG data writing
		pDataWriter->close();
	}

	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		// open DIR file to read from
		pDataReader->setFilePath(strDIRFilePathIn);
		pDataReader->open(true);
	}
	if (pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		// open DIR file to write to
		pDataWriter->setFilePath(strDIRFilePathOut);
		pDataWriter->open(true);
	}

	// write DIR data
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(pIMGEntry->getEntryOffsetInSectors());
		pDataWriter->writeUint32(pIMGEntry->getEntrySizeInSectors());
		pDataWriter->writeStringRef(pIMGEntry->getEntryName(), 24);

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	// finalize DIR data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->close();
	}
	if(pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		pDataWriter->close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}

void					IMGFormat::serializeVersion2(void)
{
	DataReader *pDataReader = DataReader::get();
	DataWriter *pDataWriter = DataWriter::get();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiBodyStart = (uiEntryCount * 32) + 8,
		uiSeek = Math::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (IMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->setFilePath(getOriginalFilePath());
		pDataReader->open(true);
	}

	// write IMG data - IMG header
	pDataWriter->writeString("VER2");
	pDataWriter->writeUint32(getEntryCount());

	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(vecNewEntryPositions[i++]);
		pDataWriter->writeUint16((uint16)ceil((float)(pIMGEntry->getEntrySize() / (float)2048.0f)));
		pDataWriter->writeUint16(0);
		pDataWriter->writeStringRef(pIMGEntry->getEntryName(), 24);

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		pDataWriter->writeString(uiPadByteCount);
	}

	// write IMG data - IMG body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataReader->setSeek(pIMGEntry->getEntryOffset());
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()), IMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->close();
	}
	if(pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		pDataWriter->close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}

void					IMGFormat::serializeVersionFastman92(void)
{
	DataReader *pDataReader = DataReader::get();
	DataWriter *pDataWriter = DataWriter::get();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiBodyStart = (uiEntryCount * 32) + 8,
		uiSeek = Math::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (IMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->setFilePath(getOriginalFilePath());
		pDataReader->open(true);
	}

	// write IMG data - IMG header
	string strIMGVersion = "VERF";
	uint32 uiArchiveVersion = 1;
	uint32 uiArchiveFlags = uiArchiveVersion;

	pDataWriter->writeStringRef(strIMGVersion);
	pDataWriter->writeUint32(uiArchiveFlags);
	pDataWriter->writeString("fastman92", 12);
	if (uiArchiveVersion == 1)
	{
		// write IMG data - IMG header
		uint32 uiCheck = 1;
		uint32 uiEntryCount = getEntryCount();
		string strReserved1 = String::zeroPad(8);

		pDataWriter->writeUint32(uiCheck);
		pDataWriter->writeUint32(uiEntryCount);
		pDataWriter->writeStringRef(strReserved1);

		// write IMG data - IMG directory
		i = 0;
		for (auto pIMGEntry : getEntries())
		{
			uint32 uiEntryFlags = 0;
			if (pIMGEntry->isCompressed())
			{
				uint32 uiCompressionValue = 0;
				switch (pIMGEntry->getCompressionAlgorithmId())
				{
				case COMPRESSION_ZLIB:
					uiCompressionValue = 1;
					break;
				case COMPRESSION_LZ4:
					uiCompressionValue = 2;
					break;
				}
				uiEntryFlags |= uiCompressionValue;
			}
			uint16 usUncompressedSizeInSectors = (uint16) ceil((float)pIMGEntry->getUncompressedSize() / (float)2048.0f);
			uint16 usPackedSizeInSectors = (uint16) pIMGEntry->getEntrySizeInSectors();
			uint16 usPaddedBytesCountInAlignedOriginalSize = (usUncompressedSizeInSectors * 2048) % 2048;
			uint16 usPaddedBytesCountInAlignedPackedSize = (usPackedSizeInSectors * 2048) % 2048;

			pDataWriter->writeUint32(vecNewEntryPositions[i++]);
			pDataWriter->writeUint16(usUncompressedSizeInSectors);
			pDataWriter->writeUint16(usPaddedBytesCountInAlignedOriginalSize);
			pDataWriter->writeUint16(usPackedSizeInSectors);
			pDataWriter->writeUint16(usPaddedBytesCountInAlignedPackedSize);
			pDataWriter->writeUint32(uiEntryFlags);
			pDataWriter->writeStringRef(pIMGEntry->getEntryName(), 40);
			pDataWriter->writeString(8);

			Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
		}

		if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
		{
			uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
			pDataWriter->writeString(uiPadByteCount);
		}

		// write IMG data - IMG body
		i = 0;
		for (auto pIMGEntry : getEntries())
		{
			pDataReader->setSeek(pIMGEntry->getEntryOffset());
			pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()), IMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize()));

			pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

			Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
		}
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->close();
	}
	if(pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		pDataWriter->close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}

void					IMGFormat::serializeVersion3_Encrypted(void)
{
	DataReader *pDataReader = DataReader::get();
	DataWriter *pDataWriter = DataWriter::get();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiNamesLength = getVersion3NamesLength(),
		uiPaddingStart = 20 + (16 * getEntryCount()) + uiNamesLength,
		uiSeek = Math::convertBytesToSectors(uiPaddingStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (IMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->setFilePath(getOriginalFilePath());
		pDataReader->open(true);
	}

	// write IMG data - IMG header
	EDataStreamType ePreviousStreamType = pDataWriter->getStreamType();
	pDataWriter->setStreamType(DATA_STREAM_MEMORY);

	uint32 uiTableByteCount = (uint32)(ceil(((float)((16 * getEntryCount()) + uiNamesLength)) / 2048.0) * 2048.0);

	pDataWriter->writeUint32(0xA94E2A52);
	pDataWriter->writeUint32(3);
	pDataWriter->writeUint32(getEntryCount());
	pDataWriter->writeUint32(uiTableByteCount);
	pDataWriter->writeUint16(16);
	pDataWriter->writeUint16(0);

	// IMG file - table
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(0);
		pDataWriter->writeUint32(pIMGEntry->getRageResourceType() == nullptr ? 0 : pIMGEntry->getRageResourceType()->getIdentifier());
		pDataWriter->writeUint32(pIMGEntry->getEntryOffsetInSectors());
		pDataWriter->writeUint16((uint16)ceil(((float)pIMGEntry->getEntrySize()) / (float)2048.0f));

		uint32 uiRemainder = pIMGEntry->getEntrySize() % 2048;
		pDataWriter->writeUint16((uint16)(pIMGEntry->getFlags() | ((uint16)(uiRemainder == 0 ? 0 : (2048 - uiRemainder)))));

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	string strEntryName;
	for (auto pIMGEntry : getEntries())
	{
		strEntryName = pIMGEntry->getEntryName();
		strEntryName.append("\0", 1);
		pDataWriter->writeStringRef(strEntryName);
	}

	if ((uiPaddingStart % 2048) != 0 && uiEntryCount > 0)
	{
		uint32 uiPadByteCount = 2048 - (uiPaddingStart % 2048);
		pDataWriter->writeString(uiPadByteCount);
	}

	// encrypt header and table
	string strData = pDataWriter->getData();

	string strHeader = IMGManager::encryptVersion3IMGString(strData.substr(0, 32));
	string strTables = IMGManager::encryptVersion3IMGString(String::zeroPad(strData.substr(20), (strData.length() - 20) + (2048 - ((strData.length() - 20) % 2048))));

	pDataWriter->setStreamType(ePreviousStreamType);
	pDataWriter->setSeek(0);
	pDataWriter->writeStringRef(strHeader);
	pDataWriter->setSeek(20);
	pDataWriter->writeStringRef(strTables);

	// IMG file - body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataReader->setSeek(pIMGEntry->getEntryOffset());
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->close();
	}
	if(pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		pDataWriter->close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}

void					IMGFormat::serializeVersion3_Unencrypted(void)
{
	DataReader *pDataReader = DataReader::get();
	DataWriter *pDataWriter = DataWriter::get();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiNamesLength = getVersion3NamesLength(),
		uiBodyStart = 20 + (16 * getEntryCount()) + uiNamesLength,
		uiSeek = Math::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (IMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->setFilePath(getOriginalFilePath());
		pDataReader->open(true);
	}

	// IMG file - header
	pDataWriter->writeUint32(0xA94E2A52);
	pDataWriter->writeUint32(3);
	pDataWriter->writeUint32(getEntryCount());
	pDataWriter->writeUint32((uint32)(ceil(((float)((16 * getEntryCount()) + uiNamesLength)) / 2048.0) * 2048));
	pDataWriter->writeUint16(16);
	pDataWriter->writeUint16(0);

	// IMG file - table
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(0);
		pDataWriter->writeUint32((pIMGEntry->getRageResourceType() == nullptr ? 0 : pIMGEntry->getRageResourceType()->getIdentifier()));
		pDataWriter->writeUint32(pIMGEntry->getEntryOffsetInSectors());
		pDataWriter->writeUint16((uint16)ceil(((float)pIMGEntry->getEntrySize()) / (float)2048.0f));

		uint32 uiRemainder = pIMGEntry->getEntrySize() % 2048;
		pDataWriter->writeUint16((uint16)((pIMGEntry->getFlags() | ((uint16)(uiRemainder == 0 ? 0 : (2048 - uiRemainder))))));

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	string strEntryName;
	for (auto pIMGEntry : getEntries())
	{
		strEntryName = pIMGEntry->getEntryName();
		strEntryName.append("\0", 1);
		pDataWriter->writeStringRef(strEntryName);
	}

	if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		pDataWriter->writeString(uiPadByteCount);
	}

	// IMG file - body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataReader->setSeek(pIMGEntry->getEntryOffset());
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		Events::triggerConst(SERIALIZE_IMG_ENTRY, this);
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE)
	{
		pDataReader->close();
	}
	if(pDataWriter->getStreamType() == DATA_STREAM_FILE)
	{
		pDataWriter->close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}

// entry fetching
void					IMGFormat::getModelAndTextureSetNamesFromEntries(
	unordered_map<IMGEntry*, vector<string>>& umapIMGModelNames,
	unordered_map<IMGEntry*, vector<string>>& umapIMGTextureSetNames
)
{
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->isModelFile())
		{
			umapIMGTextureSetNames[pIMGEntry].push_back(String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName())));
		}
		else if (pIMGEntry->isTextureFile())
		{
			umapIMGTextureSetNames[pIMGEntry].push_back(String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName())));
		}

		Events::trigger(TASK_PROGRESS);
	}
}

// extension counts
void					IMGFormat::addEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = String::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts.count(strEntryExtension) > 0)
	{
		m_umapExtensionCounts[strEntryExtension]++;
	}
	else
	{
		m_umapExtensionCounts[strEntryExtension] = 1;
	}
}
void					IMGFormat::removeEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = String::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts[strEntryExtension] > 1)
	{
		m_umapExtensionCounts[strEntryExtension]--;
	}
	else
	{
		m_umapExtensionCounts.erase(strEntryExtension);
	}
}

uint32					IMGFormat::getEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = String::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts.count(strEntryExtension) > 0)
	{
		return m_umapExtensionCounts[strEntryExtension];
	}
	else
	{
		return 0;
	}
}

void					IMGFormat::loadEntryExtensionCounts(void)
{
	m_umapExtensionCounts.clear();
	for (auto pIMGEntry : getEntries())
	{
		addEntryExtensionCount(pIMGEntry->getEntryExtension());
	}
}

vector<EFileType>			IMGFormat::getFileTypes(void)
{
	set<EFileType> setFileTypes;
	vector<EFileType> vecFileTypes;
	EFileType uiEntryType;
	for (IMGEntry *pEntry : getEntries())
	{
		uiEntryType = pEntry->getFileType();
		if (setFileTypes.count(uiEntryType) == 0)
		{
			setFileTypes.insert(uiEntryType);
			vecFileTypes.push_back(uiEntryType);
		}
	}
	return vecFileTypes;
}

vector<string>				IMGFormat::getFileTypesText(void)
{
	return StdVector::mapSorted(getFileTypes(), Format::getFileTypeText);
}

map<string, EFileType, SortByStringKey>		IMGFormat::getFileTypesAsMap(void)
{
	map<string, EFileType, SortByStringKey> mapFileTypes;
	for (EFileType uiFileType : getFileTypes())
	{
		mapFileTypes[Format::getFileTypeText(uiFileType)] = uiFileType;
	}
	return mapFileTypes;
}

vector<string>				IMGFormat::getFileVersions(void)
{
	set<string> setFileVersions;
	vector<string> vecFileVersions;
	string strVersionText;
	for (IMGEntry *pEntry : getEntries())
	{
		strVersionText = pEntry->getVersionText();
		if (setFileVersions.count(strVersionText) == 0)
		{
			setFileVersions.insert(strVersionText);
			vecFileVersions.push_back(strVersionText);
		}
	}
	StdVector::sortAZCaseInsensitive(vecFileVersions);
	return vecFileVersions;
}

map<string, pair<uint32, EFileType>, SortByStringKey>		IMGFormat::getFileTypedVersionsAsMap(void)
{
	map<string, pair<uint32, EFileType>, SortByStringKey> mapFileTypedVersions;
	set<string> setFileVersions;
	string strVersionText;
	for (IMGEntry *pEntry : getEntries())
	{
		strVersionText = pEntry->getVersionText();
		if (setFileVersions.count(strVersionText) == 0)
		{
			setFileVersions.insert(strVersionText);
			mapFileTypedVersions[strVersionText] = pair<uint32, EFileType>(pEntry->getRawVersion(), pEntry->getFileType());
		}
	}
	return mapFileTypedVersions;
}








ECompressionAlgorithm								IMGFormat::getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId(EIMGVersionFastman92CompressionAlgorithm eFastman92CompressionAlgorithmId)
{
	switch (eFastman92CompressionAlgorithmId)
	{
	case IMGFASTMAN92COMPRESSIONALGORITHM_UNKNOWN:		return COMPRESSION_UNKNOWN;
	case IMGFASTMAN92COMPRESSIONALGORITHM_UNCOMPRESSED:	return COMPRESSION_NONE;
	case IMGFASTMAN92COMPRESSIONALGORITHzLIB:			return COMPRESSION_ZLIB;
	case IMGFASTMAN92COMPRESSIONALGORITHM_LZ4:			return COMPRESSION_LZ4;
	}
	return COMPRESSION_UNKNOWN;
}

uint32			IMGFormat::getVersion3NamesLength(void)
{
	uint32 uiLength = 0;
	for (auto pIMGEntry : getEntries())
	{
		uiLength += pIMGEntry->getEntryName().length();
	}
	uiLength += getEntryCount();
	return uiLength;
}

IMGEntry*							IMGFormat::addEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	if (strEntryName == "")
	{
		strEntryName = Path::getFileName(strEntryFilePath);
	}

	string strEntryData = File::getFileContent(strEntryFilePath);

	IMGEntry *pIMGEntry = new IMGEntry(this);

	m_uiEntryCount++;

	pIMGEntry->setNewEntry(true);
	pIMGEntry->setEntryName(strEntryName);
	pIMGEntry->setEntryOffset(getNextEntryOffset());
	pIMGEntry->setEntrySize(strEntryData.length()); // this is also set inside IMGEntry::setEntryData()
	pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(strEntryName)));
	pIMGEntry->setFileCreationDate(File::getFileCreationDate(strEntryFilePath));

	if (getVersion() == IMG_3)
	{
		pIMGEntry->setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByFileExtension(Path::getFileExtension(pIMGEntry->getEntryName())));
		pIMGEntry->setFlags((uint16)IMGEntry::getVersion3IMGSizeDeduction(pIMGEntry->getEntrySize()));
	}
	else
	{
		pIMGEntry->unserializeRWVersion(nullptr, "", strEntryData.substr(0, 12));
	}

	pIMGEntry->setEntryData(strEntryData, true);

	addEntry(pIMGEntry);
	return pIMGEntry;
}

IMGEntry*							IMGFormat::addEntryViaData(string& strEntryName, string& strEntryData)
{
	IMGEntry *pIMGEntry = new IMGEntry(this);

	m_uiEntryCount++;

	pIMGEntry->setNewEntry(true);
	pIMGEntry->setEntryName(strEntryName);
	pIMGEntry->setEntryOffset(getNextEntryOffset());
	pIMGEntry->setEntrySize(strEntryData.length()); // this is also set inside IMGEntry::setEntryData()
	pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(strEntryName)));
	// todo pIMGEntry->setFileCreationDate(Math::getTimeNow());

	if (getVersion() == IMG_3)
	{
		pIMGEntry->setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByFileExtension(Path::getFileExtension(pIMGEntry->getEntryName())));
		pIMGEntry->setFlags((uint16)IMGEntry::getVersion3IMGSizeDeduction(pIMGEntry->getEntrySize()));
	}
	else
	{
		pIMGEntry->unserializeRWVersion(nullptr, "", strEntryData.substr(0, 12));
	}

	pIMGEntry->setEntryData(strEntryData, true);

	addEntry(pIMGEntry);
	return pIMGEntry;
}

void					IMGFormat::addEntry(IMGEntry *pIMGEntry)
{
	VectorPool::addEntry(pIMGEntry);

	Events::trigger(ADD_IMG_ENTRY, pIMGEntry);

	string strEntryExtension = Path::getFileExtension(pIMGEntry->getEntryName());
	addEntryExtensionCount(strEntryExtension);
	if (getEntryExtensionCount(strEntryExtension) == 1)
	{
		Events::trigger(ADD_IMG_ENTRY_FILE_EXTENSION, pIMGEntry);
	}
}

void								IMGFormat::addEntries(vector<string>& vecEntryFilePaths)
{
	for (auto strEntryFilePath : vecEntryFilePaths)
	{
		addEntryViaFile(strEntryFilePath);
	}
}

void								IMGFormat::addAllEntriesInFolder(string& strFolderPath)
{
	vector<string> vecFileNames = File::getFileNames(strFolderPath);
	for (auto strFileName : vecFileNames)
	{
		addEntryViaFile(strFileName);
	}
}

void								IMGFormat::removeEntry(IMGEntry *pIMGEntry)
{
	string strEntryExtension = Path::getFileExtension(pIMGEntry->getEntryName());
	removeEntryExtensionCount(strEntryExtension);
	if (getEntryExtensionCount(strEntryExtension) == 0)
	{
		Events::trigger(REMOVE_IMG_ENTRY_FILE_EXTENSION, pIMGEntry);
	}

	Events::trigger(REMOVE_IMG_ENTRY, pIMGEntry);

	VectorPool::removeEntry(pIMGEntry);
	m_uiEntryCount--;
}

void								IMGFormat::removeAllEntries(void)
{
	VectorPool::removeAllEntries();
	m_uiEntryCount = 0;
}

IMGEntry*							IMGFormat::replaceEntryViaFile(string& strEntryName, string& strEntryFilePath, string strNewEntryName)
{
	string strEntryData = File::getFileContent(strEntryFilePath);
	IMGEntry *pIMGEntry = replaceEntryViaData(strEntryName, strEntryData, strNewEntryName);
	pIMGEntry->setFileCreationDate(File::getFileCreationDate(strEntryFilePath));
	return pIMGEntry;
}
IMGEntry*							IMGFormat::replaceEntryViaData(string& strEntryName, string& strEntryData, string strNewEntryName)
{
	IMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (!pIMGEntry)
	{
		return nullptr;
	}

	if (strNewEntryName != "")
	{
		pIMGEntry->setEntryName(strNewEntryName);
	}

	pIMGEntry->setEntryData(strEntryData);
	return pIMGEntry;
}

uint32						IMGFormat::replaceEntries(vector<string>& vecPaths, vector<string>& vecReplacedEntryNames, vector<IMGEntry*>& vecReplacedEntries)
{
	uint32 uiReplaceCount = 0;

	for (auto strPath : vecPaths)
	{
		string strNewEntryName = Path::getFileName(strPath);
		IMGEntry *pIMGEntry = getEntryByName(strNewEntryName);
		if (!pIMGEntry)
		{
			Events::triggerConst(REPLACE_IMG_ENTRY_SKIPPED_ENTRY, this);
			continue;
		}

		// body
		string strFileContent = File::getFileContent(strPath);
		uint32 uiFileSize = strFileContent.length();
		pIMGEntry->setEntryData(strFileContent);

		// header
		pIMGEntry->setEntryName(strNewEntryName);
		pIMGEntry->setEntrySize(uiFileSize);
		if (getVersion() == IMG_3)
		{
			pIMGEntry->setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByFileExtension(Path::getFileExtension(strNewEntryName)));
		}
		else
		{
			pIMGEntry->setRWVersionByVersionCC(strFileContent.length() >= 12 ? String::unpackUint32(strFileContent.substr(8, 4), false) : 0);
		}

		uiReplaceCount++;
		vecReplacedEntryNames.push_back(strNewEntryName);
		vecReplacedEntries.push_back(pIMGEntry);

		Events::triggerConst(REPLACE_IMG_ENTRY, this);
	}

	return uiReplaceCount;
}

IMGEntry*							IMGFormat::addOrReplaceEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	IMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (pIMGEntry)
	{
		pIMGEntry = replaceEntryViaFile(strEntryName, strEntryFilePath);
	}
	else
	{
		pIMGEntry = addEntryViaFile(strEntryFilePath, strEntryName);
	}
	return pIMGEntry;
}
IMGEntry*							IMGFormat::addOrReplaceEntryViaData(string& strEntryName, string& strEntryData)
{
	IMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (pIMGEntry)
	{
		pIMGEntry = replaceEntryViaData(strEntryName, strEntryData);
	}
	else
	{
		pIMGEntry = addEntryViaData(strEntryName, strEntryData);
	}
	return pIMGEntry;
}

unordered_map<IMGEntry*, string>	IMGFormat::getAllEntriesData(void)
{
	return getEntriesData(getEntries());
}

unordered_map<IMGEntry*, string>	IMGFormat::getEntriesData(vector<IMGEntry*>& vecEntries)
{
	unordered_map<IMGEntry*, string> umapEntriesData;
	
	m_reader.setFilePath(getFilePath()); // todo - use IMG file not DIR file
	if(!openFile())
	{
		return umapEntriesData;
	}
	
	uint32 uiEntryIndex = 0;
	for (IMGEntry *pIMGEntry : vecEntries)
	{
		umapEntriesData.insert(make_pair(pIMGEntry, readEntryContent(uiEntryIndex++)));
	}
	m_reader.close();
	
	return umapEntriesData;
}

uint32			IMGFormat::getNextEntryOffset(void)
{
	if (getEntryCount() == 0)
	{
		switch (getVersion())
		{
		case IMG_1:
			return 0;
		case IMG_2:
		case IMG_3:
			return 2048;
		}
		return 0;
	}

	IMGEntry *pIMGEntry = getEntryByHighestOffset();
	return pIMGEntry->getEntryOffset() + pIMGEntry->getPaddedEntrySize();
}

vector<IMGEntry*>		IMGFormat::getEntriesByName(string strText)
{
	strText = String::toUpperCase(strText);
	vector<IMGEntry*> vecIMGEntries;
	for (auto pIMGEntry : getEntries())
	{
		if (String::isIn(String::toUpperCase(pIMGEntry->getEntryName()), strText))
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
		Events::trigger(TASK_PROGRESS);
	}
	return vecIMGEntries;
}

vector<IMGEntry*>		IMGFormat::getEntriesByExtension(string strExtension, bool bWildcard)
{
	strExtension = String::toUpperCase(strExtension);
	vector<IMGEntry*> vecIMGEntries;
	for (auto pIMGEntry : getEntries())
	{
		if (bWildcard)
		{
			if (String::isIn(String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())), strExtension))
			{
				vecIMGEntries.push_back(pIMGEntry);
			}
		}
		else
		{
			if (String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())) == strExtension)
			{
				vecIMGEntries.push_back(pIMGEntry);
			}
		}
		Events::trigger(TASK_PROGRESS);
	}
	return vecIMGEntries;
}

vector<IMGEntry*>		IMGFormat::getEntriesByVersion(uint32 uiFileTypeId, uint32 uiFileVersionId)
{
	vector<IMGEntry*> vecIMGEntries;
	for (auto pIMGEntry : getEntries())
	{
		if (pIMGEntry->getFileType() == uiFileTypeId && pIMGEntry->getRawVersion() == uiFileVersionId)
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
		Events::trigger(TASK_PROGRESS);
	}
	return vecIMGEntries;
}

IMGEntry*				IMGFormat::getEntryByName(string& strEntryName)
{
	strEntryName = String::toUpperCase(strEntryName);
	for (auto pIMGEntry : getEntries())
	{
		if (strEntryName == String::toUpperCase(pIMGEntry->getEntryName()))
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

IMGEntry*				IMGFormat::getEntryByNameWithoutExtension(string& strEntryNameWithoutExtension)
{
	strEntryNameWithoutExtension = String::toUpperCase(strEntryNameWithoutExtension);
	for (auto pIMGEntry : getEntries())
	{
		if (strEntryNameWithoutExtension == String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName())))
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

IMGEntry*				IMGFormat::getEntryByHighestOffset(void)
{
	uint32 uiHighestOffset = 0;
	IMGEntry *pHighestOffsetIMGEntry = nullptr;
	for (auto pIMGEntry : getEntries())
	{
		if (pIMGEntry->getEntryOffsetInSectors() >= uiHighestOffset)
		{
			uiHighestOffset = pIMGEntry->getEntryOffsetInSectors();
			pHighestOffsetIMGEntry = pIMGEntry;
		}
	}
	return pHighestOffsetIMGEntry;
}

uint32			IMGFormat::getEntryCountForName(string& strEntryName)
{
	strEntryName = String::toUpperCase(strEntryName);
	uint32 uiNameCount = 0;
	for (auto pIMGEntry : getEntries())
	{
		if (strEntryName == String::toUpperCase(pIMGEntry->getEntryName()))
		{
			uiNameCount++;
		}
	}
	return uiNameCount;
}

void					IMGFormat::unsetNewAndReplacedFlagForAllEntries(void)
{
	for (auto pIMGEntry : getEntries())
	{
		pIMGEntry->setNewEntry(false);
		pIMGEntry->setReplacedEntry(false);
	}
}

uint32			IMGFormat::getEntryPaddedSize(uint32 uiDataLength)
{
	return (uint32)(ceil((float)uiDataLength / (float)2048.0f) * (float)2048.0f);
}

uint32			IMGFormat::merge(string& strSecondIMGPath, vector<string>& vecImportedEntryNames)
{
	DataReader *pDataReader = DataReader::get();

	// parse second IMG file for entry information
	IMGFormat imgFileIn(strSecondIMGPath);
	imgFileIn.setFilePath(strSecondIMGPath);
	if (!imgFileIn.unserialize())
	{
		imgFileIn.unload();
		return 0;
	}

	// open second IMG file to read entry body data from
	pDataReader->setFilePath(getFilePath());
	pDataReader->open(true);

	// import entries from second IMG into first IMG
	bool bVersion3IMG = getVersion() == IMG_3;
	for (auto pInEntry : imgFileIn.getEntries())
	{
		// entry header data
		IMGEntry *pOutEntry = new IMGEntry(this);
		pOutEntry->setEntryName(pInEntry->getEntryName());
		pOutEntry->setEntrySize(pInEntry->getEntrySize());
		if (bVersion3IMG)
		{
			pOutEntry->setRageResourceType(pInEntry->getRageResourceType());
			pOutEntry->setFlags(pInEntry->getFlags());
		}
		else
		{
			pOutEntry->setRWVersion(pInEntry->getRWVersion());
		}
		addEntry(pOutEntry);
		m_uiEntryCount++;

		// entry body data
		pDataReader->setSeek(pInEntry->getEntryOffset());
		string strEntryData = pDataReader->readString(pInEntry->getEntrySize());
		pOutEntry->setEntryData(strEntryData, true);

		vecImportedEntryNames.push_back(pOutEntry->getEntryName());

		Events::trigger(TASK_PROGRESS);
	}

	// finalize
	pDataReader->close();

	uint32 uiImportedEntryCount = imgFileIn.getEntryCount();
	imgFileIn.unload();

	return uiImportedEntryCount;
}

void					IMGFormat::split(vector<IMGEntry*>& vecIMGEntries, string& strOutPath, EIMGVersion EIMGVersion)
{
	IMGFormat *pIMGFile = new IMGFormat;
	pIMGFile->setVersion(EIMGVersion);
	pIMGFile->setFilePath(getFilePath());

	bool bVersion3IMG = EIMGVersion == IMG_3;
	for (auto pIMGEntry : vecIMGEntries)
	{
		IMGEntry *pIMGEntry2 = new IMGEntry(pIMGFile);
		pIMGEntry2->setEntryName(pIMGEntry->getEntryName());
		pIMGEntry2->setEntryOffsetInSectors(pIMGEntry->getEntryOffsetInSectors());
		pIMGEntry2->setEntrySize(pIMGEntry->getEntrySize());
		pIMGFile->addEntry(pIMGEntry2);

		if (bVersion3IMG)
		{
			pIMGEntry2->setRageResourceType(pIMGEntry->getRageResourceType());
			pIMGEntry2->setFlags(pIMGEntry->getFlags());
		}
		else
		{
			pIMGEntry2->setRWVersion(pIMGEntry->getRWVersion());
		}

		Events::triggerConst(SPLIT_IMG_ENTRY, this);
	}

	pIMGFile->serializeViaFile(strOutPath);

	pIMGFile->unload();
	delete pIMGFile;
}

void					IMGFormat::exportSingle(IMGEntry *pIMGEntry, string& strFolderPath)
{
	if (!pIMGEntry->canBeRead())
	{
		return;
	}

	strFolderPath = Path::addSlashToEnd(strFolderPath);

	File::storeFile(strFolderPath + pIMGEntry->getEntryName(), pIMGEntry->getEntryData(), false, true);
}

void					IMGFormat::exportMultiple(vector<IMGEntry*>& vecIMGEntries, string strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getIMGFilePath());
	if(!openFile())
	{
		return;
	}
	
	uint32 uiEntryIndex = 0;
	for (IMGEntry *pIMGEntry : vecIMGEntries)
	{
		if (!pIMGEntry->canBeRead())
		{
			uiEntryIndex++;
			continue;
		}
		
		File::storeFile(strFolderPath + pIMGEntry->getEntryName(), readEntryContent(uiEntryIndex++), false, true);
		
		Events::trigger(TASK_PROGRESS);
	}

	m_reader.close();
}

void					IMGFormat::exportAll(string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getIMGFilePath());
	if(!openFile())
	{
		return;
	}
	
	uint32 uiEntryIndex = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (!pIMGEntry->canBeRead())
		{
			uiEntryIndex++;
			continue;
		}
		
		File::storeFile(strFolderPath + pIMGEntry->getEntryName(), readEntryContent(uiEntryIndex++), false, true);
		
		Events::trigger(TASK_PROGRESS);
	}
	m_reader.close();
}

IMGFormat*				IMGFormat::clone(string& strClonedIMGPath)
{
	serializeViaFile(strClonedIMGPath);
	return IMGManager::get()->parseViaFile(strClonedIMGPath);
}

vector<IMGEntry*>		IMGFormat::getUnknownVersionEntries(void)
{
	vector<IMGEntry*> vecIMGEntries;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->doesHaveUnknownVersion())
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
	}
	return vecIMGEntries;
}

uint32			IMGFormat::getEntryCountForCompressionType(ECompressionAlgorithm ECompressionAlgorithmValue)
{
	uint32 uiCount = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->getCompressionAlgorithmId() == ECompressionAlgorithmValue)
		{
			uiCount++;
		}
	}
	return uiCount;
}

vector<string>			IMGFormat::getEntryNames(void)
{
	vector<string> vecEntryNames;
	vecEntryNames.resize(getEntryCount());
	uint32 i = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		vecEntryNames[i] = pIMGEntry->getEntryName();
		++i;
	}
	return vecEntryNames;
}