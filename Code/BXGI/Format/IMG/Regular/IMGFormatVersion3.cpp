#include "IMGFormatVersion3.h"
#include "Format/IMG/Regular/IMGManager.h"
#include "Format/IMG/Regular/Raw/IMGEntry_Version3.h"
#include "Format/IMG/Regular/Raw/IMGFormat_Version3_Header1.h"
#include "Format/IMG/Regular/IMGEntry.h"
#include "Engine/RAGE/RageResourceTypeManager.h"
#include "Static/Math.h"
#include "Static/Path.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"
#include "../BXGI/Event/EEvent.h"

#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IMGFormatVersion3::IMGFormatVersion3(void) :
	IMGFormat(IMG_3),
	m_iUndecryptedPositionOffset(0)
{
}

IMGFormatVersion3::IMGFormatVersion3(string strFilePathOrData, bool bParam1IsFilePath) :
	IMGFormat(IMG_3, strFilePathOrData, bParam1IsFilePath),
	m_iUndecryptedPositionOffset(0)
{
}

// serialization
void					IMGFormatVersion3::_unserialize(void)
{
	IMGFormat::_unserializeBefore();

	if (isEncrypted())
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
		vector<IMGEntry*>& rvecIMGEntries = VectorPool::getEntries();
		rvecIMGEntries.clear();
		rvecIMGEntries.resize(uiEntryCount);

		for (uint32 i = 0; i < uiEntryCount; i++)
		{
			IMGEntry *pIMGEntry = new IMGEntry;
			rvecIMGEntries[i] = pIMGEntry;
			pIMGEntry->setIMGFile(this);
			pIMGEntry->unserializeVersion3(pRGIMGActiveEntry++);
			Events::triggerDefault(UNSERIALIZE_IMG_ENTRY, (IMGFormat*)this);
		}

		// read IMG entry names
		//pDataReader->setStreamType(ePreviousStreamType);
		//pDataReader->setSeek();

		for (uint32 i = 0; i < uiEntryCount; i++)
		{
			rvecIMGEntries[i]->setEntryName(pDataReader2->readStringUntilZero());
			rvecIMGEntries[i]->setEntryExtension(String::toUpperCase(Path::getFileExtension(rvecIMGEntries[i]->getEntryName())));
			Events::triggerDefault(UNSERIALIZE_IMG_ENTRY, (IMGFormat*)this);
		}

		setUndecryptedPositionOffset((strIMGHeaderUnencrypted.length() + strIMGTableUnencrypted.length()) - (strIMGHeaderEncrypted.length() + strIMGTableEncrypted.length()));

		// restore
		//pDataReader->setStreamType(ePreviousStreamType);
		pDataReader2->reset();
	}
	else
	{
		// read header 1
		RG_IMGFormat_Version3_Header1 *pHeader1 = m_reader.readStruct<RG_IMGFormat_Version3_Header1>();
		uint32 uiEntryCount = pHeader1->m_uiEntryCount;

		// load data from file into RG structs
		RG_IMGEntry_Version3
			*pRGIMGEntries = m_reader.readStructMultiple<RG_IMGEntry_Version3>(uiEntryCount),
			*pRGIMGActiveEntry = pRGIMGEntries;

		// copy IMG tables RG structs into wrapper structs
		vector<IMGEntry*>& rvecIMGEntries = VectorPool::getEntries();
		rvecIMGEntries.clear();
		rvecIMGEntries.resize(uiEntryCount);

		for (uint32 i = 0; i < uiEntryCount; i++)
		{
			IMGEntry *pIMGEntry = new IMGEntry;
			rvecIMGEntries[i] = pIMGEntry;
			pIMGEntry->setIMGFile(this);
			pIMGEntry->unserializeVersion3(pRGIMGActiveEntry++);
			Events::triggerDefault(UNSERIALIZE_IMG_ENTRY, (IMGFormat*)this);
		}

		// read IMG entry names
		for (uint32 i = 0; i < uiEntryCount; i++)
		{
			rvecIMGEntries[i]->setEntryName(m_reader.readStringUntilZero());
			rvecIMGEntries[i]->setEntryExtension(String::toUpperCase(Path::getFileExtension(rvecIMGEntries[i]->getEntryName())));
			Events::triggerDefault(UNSERIALIZE_IMG_ENTRY, (IMGFormat*)this);
		}

		// clean up
		delete pHeader1;
		delete[] pRGIMGEntries;
	}

	IMGFormat::_unserializeAfter();
}

void					IMGFormatVersion3::_serialize(void)
{
	bool bContainsAnEntry = getEntryCount() > 0;

	if (isEncrypted())
	{
		// fetch new seek positions for all IMG entries
		uint32
			uiEntryCount = getEntryCount(),
			uiNamesLength = getVersion3NamesLength(),
			uiPaddingStart = 20 + (16 * getEntryCount()) + uiNamesLength,
			uiSeek = Math::convertBytesToSectors(uiPaddingStart),
			i = 0;
		vector<uint32> vecNewEntryPositions;
		vecNewEntryPositions.resize(uiEntryCount);
		for (IMGEntry *pIMGEntry : VectorPool::getEntries())
		{
			vecNewEntryPositions[i] = uiSeek;
			uiSeek += pIMGEntry->getEntrySizeInSectors();
			i++;
		}

		// open IMG file to read from (IMG file to write to is already open in DataWriter)
		if (m_reader.getStreamType() == DATA_STREAM_FILE && bContainsAnEntry)
		{
			m_reader.setFilePath(getIMGFilePath());
			m_reader.open(true);
		}

		// write IMG data - IMG header
		EDataStreamType ePreviousStreamType = m_writer.getStreamType();
		m_writer.setStreamType(DATA_STREAM_MEMORY);
		m_writer.reset();

		uint32 uiTableByteCount = (uint32)(ceil(((float)((16 * getEntryCount()) + uiNamesLength)) / 2048.0) * 2048.0);

		m_writer.writeUint32(0xA94E2A52);
		m_writer.writeUint32(3);
		m_writer.writeUint32(getEntryCount());
		m_writer.writeUint32(uiTableByteCount);
		m_writer.writeUint16(16);
		m_writer.writeUint16(0);

		// IMG file - table
		i = 0;
		for (IMGEntry *pIMGEntry : VectorPool::getEntries())
		{
			m_writer.writeUint32(0);
			m_writer.writeUint32(pIMGEntry->getRageResourceType() == nullptr ? 0 : pIMGEntry->getRageResourceType()->getIdentifier());
			m_writer.writeUint32(vecNewEntryPositions[i++]);
			m_writer.writeUint16(pIMGEntry->getEntrySizeInSectors());

			uint32 uiRemainder = pIMGEntry->getEntrySize() % 2048;
			m_writer.writeUint16((uint16)(pIMGEntry->getFlags() | ((uint16)(uiRemainder == 0 ? 0 : (2048 - uiRemainder)))));

			Events::triggerDefault(TASK_PROGRESS);
		}

		string strEntryName;
		for (IMGEntry *pIMGEntry : VectorPool::getEntries())
		{
			strEntryName = pIMGEntry->getEntryName();
			strEntryName.append("\0", 1);
			m_writer.writeStringRef(strEntryName);
		}

		if ((uiPaddingStart % 2048) != 0 && uiEntryCount > 0)
		{
			uint32 uiPadByteCount = 2048 - (uiPaddingStart % 2048);
			m_writer.writeString(uiPadByteCount);
		}

		// encrypt header and table
		string strData = m_writer.getData();

		string strHeader = IMGManager::encryptVersion3IMGString(strData.substr(0, 32)).substr(0, 20);
		string strTables = IMGManager::encryptVersion3IMGString(String::zeroPad(strData.substr(20), strData.length())).substr(0, strData.length() - 20);

		m_writer.setStreamType(ePreviousStreamType);
		m_writer.setSeek(0);
		m_writer.writeStringRef(strHeader);
		m_writer.writeStringRef(strTables);

		// IMG file - body
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
	else
	{
		// fetch new seek positions for all IMG entries
		uint32
			uiEntryCount = getEntryCount(),
			uiNamesLength = getVersion3NamesLength(),
			uiBodyStart = 20 + (16 * getEntryCount()) + uiNamesLength,
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

		// open IMG file to read from (IMG file to write to is already open in DataWriter)
		if (m_reader.getStreamType() == DATA_STREAM_FILE && bContainsAnEntry)
		{
			m_reader.setFilePath(getIMGFilePath());
			m_reader.open(true);
		}

		// IMG file - header
		m_writer.writeUint32(0xA94E2A52);
		m_writer.writeUint32(3);
		m_writer.writeUint32(getEntryCount());
		m_writer.writeUint32((uint32)(ceil(((float)((16 * getEntryCount()) + uiNamesLength)) / 2048.0) * 2048));
		m_writer.writeUint16(16);
		m_writer.writeUint16(0);

		// IMG file - table
		for (IMGEntry *pIMGEntry : VectorPool::getEntries())
		{
			m_writer.writeUint32(0);
			m_writer.writeUint32((pIMGEntry->getRageResourceType() == nullptr ? 0 : pIMGEntry->getRageResourceType()->getIdentifier()));
			m_writer.writeUint32(pIMGEntry->getEntryOffsetInSectors());
			m_writer.writeUint16(pIMGEntry->getEntrySizeInSectors());

			uint32 uiRemainder = pIMGEntry->getEntrySize() % 2048;
			m_writer.writeUint16((uint16)((pIMGEntry->getFlags() | ((uint16)(uiRemainder == 0 ? 0 : (2048 - uiRemainder))))));

			Events::triggerDefault(TASK_PROGRESS);
		}

		string strEntryName;
		for (IMGEntry *pIMGEntry : VectorPool::getEntries())
		{
			strEntryName = pIMGEntry->getEntryName();
			strEntryName.append("\0", 1);
			m_writer.writeStringRef(strEntryName);
		}

		if ((uiBodyStart % 2048) != 0 && uiEntryCount > 0)
		{
			uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
			m_writer.writeString(uiPadByteCount);
		}

		// IMG file - body
		i = 0;
		for (IMGEntry *pIMGEntry : VectorPool::getEntries())
		{
			m_reader.setSeek(pIMGEntry->getEntryOffset());
			m_writer.writeString(m_reader.readString(pIMGEntry->getEntrySize()));

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
}