#include "IMGFormatVersion2.h"
#include "Format/IMG/Regular/Raw/IMGFormat_Version2_Header1.h"
#include "Format/IMG/Regular/IMGEntry.h"
#include "Static/Math.h"
#include "Static/Path.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"
#include "../BXGI/Event/EEvent.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IMGFormatVersion2::IMGFormatVersion2(void) :
	IMGFormat(IMG_2)
{
}

IMGFormatVersion2::IMGFormatVersion2(string strFilePathOrData, bool bParam1IsFilePath) :
	IMGFormat(IMG_2, strFilePathOrData, bParam1IsFilePath)
{
}

// serialization
void					IMGFormatVersion2::_unserialize(void)
{
	IMGFormat::_unserializeBefore();

	// read header 1
	RG_IMGFormat_Version2_Header1 *pHeader1 = m_reader.readStruct<RG_IMGFormat_Version2_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;

	// load data from file into RG structs
	RG_IMGEntry_Version2
		*pRGIMGEntries = m_reader.readStructMultiple<RG_IMGEntry_Version2>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy RG structs into wrapper structs
	vector<IMGEntry*>& rvecIMGEntries = VectorPool::getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);

	for (uint64 i = 0; i < uiEntryCount; i++)
	{
		IMGEntry *pIMGEntry = new IMGEntry;
		rvecIMGEntries[(unsigned int)i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion2(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())));
		Events::triggerDefault(UNSERIALIZE_IMG_ENTRY, (IMGFormat*)this);
	}

	// clean up
	delete pHeader1;
	delete[] pRGIMGEntries;

	IMGFormat::_unserializeAfter();
}

void					IMGFormatVersion2::_serialize(void)
{
	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiBodyStart = (uiEntryCount * 32) + 8,
		uiSeek = Math::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (IMGEntry *pIMGEntry : VectorPool::getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	bool bContainsAnEntry = getEntryCount() > 0;

	// open IMG file to read from (IMG file to write to is already open in DataWriter)
	if (m_reader.getStreamType() == DATA_STREAM_FILE && bContainsAnEntry)
	{
		m_reader.setFilePath(getIMGFilePath());
		m_reader.open(true);
	}

	// write IMG data - IMG header
	m_writer.writeString("VER2");
	m_writer.writeUint32(getEntryCount());

	i = 0;
	for (IMGEntry *pIMGEntry : VectorPool::getEntries())
	{
		m_writer.writeUint32(vecNewEntryPositions[i++]);
		m_writer.writeUint16(pIMGEntry->getEntrySizeInSectors());
		m_writer.writeUint16(pIMGEntry->getEntrySizeInSectors());
		m_writer.writeStringRef(pIMGEntry->getEntryName(), 24);

		Events::triggerDefault(TASK_PROGRESS);
	}

	if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		m_writer.writeString(uiPadByteCount);
	}

	// write IMG data - IMG body
	i = 0;
	for (IMGEntry *pIMGEntry : VectorPool::getEntries())
	{
		m_reader.setSeek(pIMGEntry->getEntryOffset());
		m_writer.writeString(m_reader.readString(pIMGEntry->getEntrySize()), pIMGEntry->getPaddedEntrySize());

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		Events::triggerDefault(TASK_PROGRESS);
	}

	// finalize IMG data reading/writing
	if (m_reader.getStreamType() == DATA_STREAM_FILE && bContainsAnEntry)
	{
		m_reader.close();
	}
	if (m_writer.getStreamType() == DATA_STREAM_FILE)
	{
		m_writer.close(); // optionally called here, DataWriter::close() will be called by Format too.
	}
}