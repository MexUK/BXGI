#include "CRWSection_Texture.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_Texture::CRWSection_Texture(void) :
	m_usFilterFlags(0),
	m_usUnknown1(0)
{
	setSectionId(RW_SECTION_TEXTURE);
}

void							CRWSection_Texture::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_usFilterFlags = pDataReader->readUint16();
	m_usUnknown1 = pDataReader->readUint16();
}

void							CRWSection_Texture::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint16(m_usFilterFlags);
	pDataWriter->writeUint16(m_usUnknown1);
}