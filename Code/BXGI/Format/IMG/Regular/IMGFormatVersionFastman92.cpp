#include "IMGFormatVersionFastman92.h"
#include "Format/IMG/Fastman92/IMGFormat_VersionFastman92_Header1.h"
#include "Format/IMG/Fastman92/IMGFormat_VersionFastman92_Header2.h"
#include "Format/IMG/Fastman92/IMGEntry_Fastman92.h"
#include "Format/IMG/Regular/IMGEntry.h"
#include "Exception/EExceptionCode.h"
#include "Static/Math.h"
#include "Static/Path.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"
#include "../BXGI/Event/EEvent.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IMGFormatVersionFastman92::IMGFormatVersionFastman92(void)
{
}

IMGFormatVersionFastman92::IMGFormatVersionFastman92(string strFilePathOrData, bool bParam1IsFilePath) :
	IMGFormat(IMG_FASTMAN92, strFilePathOrData, bParam1IsFilePath)
{
}

// serialization
void					IMGFormatVersionFastman92::_unserialize(void)
{
	IMGFormat::_unserializeBefore();

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

	IMGFormat::_unserializeAfter();
}

void					IMGFormatVersionFastman92::_serialize(void)
{
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
	if (m_reader.getStreamType() == DATA_STREAM_FILE)
	{
		m_reader.setFilePath(getIMGFilePath());
		m_reader.open(true);
	}

	// write IMG data - IMG header
	string strIMGVersion = "VERF";
	uint32 uiArchiveVersion = 1;
	uint32 uiArchiveFlags = uiArchiveVersion;

	m_writer.writeStringRef(strIMGVersion);
	m_writer.writeUint32(uiArchiveFlags);
	m_writer.writeString("fastman92", 12);
	if (uiArchiveVersion == 1)
	{
		// write IMG data - IMG header
		uint32 uiCheck = 1;
		uint32 uiEntryCount = getEntryCount();
		string strReserved1 = String::zeroPad(8);

		m_writer.writeUint32(uiCheck);
		m_writer.writeUint32(uiEntryCount);
		m_writer.writeStringRef(strReserved1);

		// write IMG data - IMG directory
		i = 0;
		for (IMGEntry *pIMGEntry : getEntries())
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
			uint16 usUncompressedSizeInSectors = pIMGEntry->getEntrySizeInSectors();
			uint16 usPackedSizeInSectors = (uint16)pIMGEntry->getEntrySizeInSectors();
			uint16 usPaddedBytesCountInAlignedOriginalSize = (usUncompressedSizeInSectors * 2048) % 2048;
			uint16 usPaddedBytesCountInAlignedPackedSize = (usPackedSizeInSectors * 2048) % 2048;

			m_writer.writeUint32(vecNewEntryPositions[i++]);
			m_writer.writeUint16(usUncompressedSizeInSectors);
			m_writer.writeUint16(usPaddedBytesCountInAlignedOriginalSize);
			m_writer.writeUint16(usPackedSizeInSectors);
			m_writer.writeUint16(usPaddedBytesCountInAlignedPackedSize);
			m_writer.writeUint32(uiEntryFlags);
			m_writer.writeStringRef(pIMGEntry->getEntryName(), 40);
			m_writer.writeString(8);

			Events::trigger(TASK_PROGRESS);
		}

		if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
		{
			uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
			m_writer.writeString(uiPadByteCount);
		}

		// write IMG data - IMG body
		i = 0;
		for (IMGEntry *pIMGEntry : getEntries())
		{
			m_reader.setSeek(pIMGEntry->getEntryOffset());
			m_writer.writeString(m_reader.readString(pIMGEntry->getEntrySize()), IMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize()));

			pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

			Events::trigger(TASK_PROGRESS);
		}
	}

	// finalize IMG data reading/writing
	if (m_reader.getStreamType() == DATA_STREAM_FILE)
	{
		m_reader.close();
	}
	if (m_writer.getStreamType() == DATA_STREAM_FILE)
	{
		m_writer.close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}