#include "CRWSection_Frame.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_Frame::CRWSection_Frame(void)
{
	setSectionId(RW_SECTION_FRAME);
}

void							CRWSection_Frame::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							CRWSection_Frame::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeStringRef(m_strData);
}