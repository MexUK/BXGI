#include "CRWSection_Frame.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_Frame::CRWSection_Frame(void)
{
	setSectionId(RW_SECTION_FRAME);
}

void							CRWSection_Frame::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							CRWSection_Frame::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strData);
}