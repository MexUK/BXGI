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
	//m_strFrameData = pDataReader->readString(56 * m_uiFrameCount);

	m_vecMatRow1.resize(m_uiFrameCount);
	m_vecMatRow2.resize(m_uiFrameCount);
	m_vecMatRow3.resize(m_uiFrameCount);
	m_vecPosition.resize(m_uiFrameCount);
	m_uiCurrentFrameIndex.resize(m_uiFrameCount);
	m_uiMatrixCreationFlags.resize(m_uiFrameCount);

	for (uint32 i = 0, j = m_uiFrameCount; i < j; i++)
	{
		m_vecMatRow1[i] = pDataReader->readVector3D();
		m_vecMatRow2[i] = pDataReader->readVector3D();
		m_vecMatRow3[i] = pDataReader->readVector3D();
		m_vecPosition[i] = pDataReader->readVector3D();
		m_uiCurrentFrameIndex[i] = pDataReader->readUint32();
		m_uiMatrixCreationFlags[i] = pDataReader->readUint32();
	}
}

void							RWSection_FrameList::_serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	pDataWriter->writeUint32(m_uiFrameCount);
	//pDataWriter->writeStringRef(m_strFrameData);
	for (uint32 i = 0, j = m_uiFrameCount; i < j; i++)
	{
		pDataWriter->writeVector3D(m_vecMatRow1[i]);
		pDataWriter->writeVector3D(m_vecMatRow2[i]);
		pDataWriter->writeVector3D(m_vecMatRow3[i]);
		pDataWriter->writeVector3D(m_vecPosition[i]);
		pDataWriter->writeUint32(m_uiCurrentFrameIndex[i]);
		pDataWriter->writeUint32(m_uiMatrixCreationFlags[i]);
	}
}