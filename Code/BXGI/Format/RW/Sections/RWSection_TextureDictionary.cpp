#include "RWSection_TextureDictionary.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_TextureDictionary::RWSection_TextureDictionary(void) :
	m_usDeviceId(0)
{
	setSectionId(RW_SECTION_TEXTURE_DICTIONARY);
}

// serialization
void							RWSection_TextureDictionary::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	if (m_uiSectionRWVersion <= 0x1803FFFF) // todo - change from 3.6.0.3 to 3.6.0.0
	{
		uint16 usTextureCount = pDataReader->readUint16();
		uint16 usDeviceId = pDataReader->readUint16();
	}
	else
	{
		uint32 uiTextureCount = pDataReader->readUint32();
	}
}

void							RWSection_TextureDictionary::_serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	uint32 uiTextureCount = getSectionCountByType(RW_SECTION_TEXTURE_NATIVE, false);

	if (m_uiSectionRWVersion <= 0x1803FFFF) // todo - change from 3.6.0.3 to 3.6.0.0
	{
		uint16 usDeviceId = 0;

		pDataWriter->writeUint16((uint16)uiTextureCount);
		pDataWriter->writeUint16(usDeviceId);
	}
	else
	{
		pDataWriter->writeUint32(uiTextureCount);
	}
}