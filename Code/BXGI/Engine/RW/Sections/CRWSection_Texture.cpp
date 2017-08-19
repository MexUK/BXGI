#include "CRWSection_Texture.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

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
	CDataReader *pDataReader = CDataReader::get();

	m_usFilterFlags = pDataReader->readUint16();
	m_usUnknown1 = pDataReader->readUint16();
}

void							CRWSection_Texture::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeUint16(m_usFilterFlags);
	pDataWriter->writeUint16(m_usUnknown1);
}