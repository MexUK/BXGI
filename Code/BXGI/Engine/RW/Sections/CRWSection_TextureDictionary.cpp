#include "CRWSection_TextureDictionary.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_TextureDictionary::CRWSection_TextureDictionary(void) :
	m_usDeviceId(0)
{
	setSectionId(RW_SECTION_TEXTURE_DICTIONARY);
}

void							CRWSection_TextureDictionary::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

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

void							CRWSection_TextureDictionary::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

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