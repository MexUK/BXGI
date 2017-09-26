#include "IMGFormatVersion1.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version1Or2.h"
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

IMGFormatVersion1::IMGFormatVersion1(void)
{
}

IMGFormatVersion1::IMGFormatVersion1(string strFilePathOrData, bool bParam1IsFilePath) :
	IMGFormat(IMG_1, strFilePathOrData, bParam1IsFilePath)
{
}

// serialization
void					IMGFormatVersion1::_unserialize(void)
{
	IMGFormat::_unserializeBefore();

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

	IMGFormat::_unserializeAfter();
}

void					IMGFormatVersion1::_serialize(void)
{
	string
		strDIRFilePathIn = getDIRFilePath(),
		strIMGFilePathIn = getIMGFilePath(),
		strIMGFilePathOut = m_writer.getFilePath(),
		strDIRFilePathOut = Path::replaceFileExtensionWithCase(strIMGFilePathOut, "DIR");

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (m_reader.getStreamType() == DATA_STREAM_FILE)
	{
		m_reader.setFilePath(strIMGFilePathIn);
		m_reader.open(true);
	}

	// write IMG data
	uint32 uiSeek = 0;
	for (auto pIMGEntry : getEntries())
	{
		uint32 uiEntryByteCountPadded = IMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize());

		m_reader.setSeek(pIMGEntry->getEntryOffset());
		m_writer.writeString(m_reader.readString(pIMGEntry->getEntrySize()), uiEntryByteCountPadded);

		pIMGEntry->setEntryOffset(Math::convertBytesToSectors(uiSeek));
		uiSeek += uiEntryByteCountPadded;

		Events::trigger(TASK_PROGRESS);
	}

	if (m_reader.getStreamType() == DATA_STREAM_FILE)
	{
		// finalize IMG data reading
		m_reader.close();
	}
	if (m_writer.getStreamType() == DATA_STREAM_FILE)
	{
		// finalize IMG data writing
		m_writer.close();
	}

	if (m_reader.getStreamType() == DATA_STREAM_FILE)
	{
		// open DIR file to read from
		m_reader.setFilePath(strDIRFilePathIn);
		m_reader.open(true);
	}
	if (m_writer.getStreamType() == DATA_STREAM_FILE)
	{
		// open DIR file to write to
		m_writer.setFilePath(strDIRFilePathOut);
		m_writer.open(true);
	}

	// write DIR data
	for (auto pIMGEntry : getEntries())
	{
		m_writer.writeUint32(pIMGEntry->getEntryOffsetInSectors());
		m_writer.writeUint32(pIMGEntry->getEntrySizeInSectors());
		m_writer.writeStringRef(pIMGEntry->getEntryName(), 24);

		Events::trigger(TASK_PROGRESS);
	}

	// finalize DIR data reading/writing
	if (m_reader.getStreamType() == DATA_STREAM_FILE)
	{
		m_reader.close();
	}
	if (m_writer.getStreamType() == DATA_STREAM_FILE)
	{
		m_writer.close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}