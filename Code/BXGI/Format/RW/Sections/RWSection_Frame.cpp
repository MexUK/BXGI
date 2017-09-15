#include "RWSection_Frame.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Frame::RWSection_Frame(void)
{
	setSectionId(RW_SECTION_FRAME);
}

// serialization
void							RWSection_Frame::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							RWSection_Frame::_serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	pDataWriter->writeStringRef(m_strData);
}