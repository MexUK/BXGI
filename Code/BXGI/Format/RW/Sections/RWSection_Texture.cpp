#include "RWSection_Texture.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Texture::RWSection_Texture(void) :
	m_usFilterFlags(0),
	m_usUnknown1(0)
{
	setSectionId(RW_SECTION_TEXTURE);
}

// serialization
void							RWSection_Texture::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_usFilterFlags = pDataReader->readUint16();
	m_usUnknown1 = pDataReader->readUint16();
}

void							RWSection_Texture::_serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	pDataWriter->writeUint16(m_usFilterFlags);
	pDataWriter->writeUint16(m_usUnknown1);
}