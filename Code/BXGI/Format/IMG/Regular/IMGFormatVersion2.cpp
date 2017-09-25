#include "IMGFormatVersion2.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version1Or2.h"
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

IMGFormatVersion2::IMGFormatVersion2(void)
{
}

IMGFormatVersion2::IMGFormatVersion2(string strFilePathOrData, bool bParam1IsFilePath) :
	IMGFormat(strFilePathOrData, bParam1IsFilePath)
{
}

// serialization
void					IMGFormatVersion2::_unserialize(void)
{
	IMGFormat::_unserialize();

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
	m_writer.writeString("VER2");
	m_writer.writeUint32(getEntryCount());

	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		m_writer.writeUint32(vecNewEntryPositions[i++]);
		m_writer.writeUint16((uint16)ceil((float)(pIMGEntry->getEntrySize() / (float)2048.0f)));
		m_writer.writeUint16(0);
		m_writer.writeStringRef(pIMGEntry->getEntryName(), 24);

		Events::trigger(TASK_PROGRESS);
	}

	if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		m_writer.writeString(uiPadByteCount);
	}

	// write IMG data - IMG body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		m_reader.setSeek(pIMGEntry->getEntryOffset());
		m_writer.writeString(m_reader.readString(pIMGEntry->getEntrySize()), IMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		Events::trigger(TASK_PROGRESS);
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