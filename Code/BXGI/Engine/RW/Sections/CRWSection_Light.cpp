#include "CRWSection_Light.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_Light::CRWSection_Light(void)
{
	setSectionId(RW_SECTION_LIGHT);
}

void							CRWSection_Light::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_strData = pDataReader->readString(24);
}

void							CRWSection_Light::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeStringRef(m_strData, 24);
}