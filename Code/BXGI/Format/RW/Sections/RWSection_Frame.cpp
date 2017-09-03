#include "RWSection_Frame.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Frame::RWSection_Frame(void)
{
	setSectionId(RW_SECTION_FRAME);
}

void							RWSection_Frame::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							RWSection_Frame::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strData);
}