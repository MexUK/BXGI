#include "RWSection_Light.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Light::RWSection_Light(void)
{
	setSectionId(RW_SECTION_LIGHT);
}

// serialization
void							RWSection_Light::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_strData = pDataReader->readString(24);
}

void							RWSection_Light::_serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strData, 24);
}