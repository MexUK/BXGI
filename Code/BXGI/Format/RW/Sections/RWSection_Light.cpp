#include "RWSection_Light.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Light::RWSection_Light(void)
{
	setSectionId(RW_SECTION_LIGHT);
}

void							RWSection_Light::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_strData = pDataReader->readString(24);
}

void							RWSection_Light::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strData, 24);
}