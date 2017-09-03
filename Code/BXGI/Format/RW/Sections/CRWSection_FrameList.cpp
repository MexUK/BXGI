#include "CRWSection_FrameList.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_FrameList::CRWSection_FrameList(void) :
	m_uiFrameCount(0)
{
	setSectionId(RW_SECTION_FRAME_LIST);
}

void							CRWSection_FrameList::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_uiFrameCount = pDataReader->readUint32();
	m_strFrameData = pDataReader->readString(56 * m_uiFrameCount);
}

void							CRWSection_FrameList::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiFrameCount);
	pDataWriter->writeStringRef(m_strFrameData);
}