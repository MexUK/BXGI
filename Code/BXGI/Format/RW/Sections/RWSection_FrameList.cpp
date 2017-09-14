#include "RWSection_FrameList.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_FrameList::RWSection_FrameList(void) :
	m_uiFrameCount(0)
{
	setSectionId(RW_SECTION_FRAME_LIST);
}

// serialization
void							RWSection_FrameList::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_uiFrameCount = pDataReader->readUint32();
	m_strFrameData = pDataReader->readString(56 * m_uiFrameCount);
}

void							RWSection_FrameList::_serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiFrameCount);
	pDataWriter->writeStringRef(m_strFrameData);
}