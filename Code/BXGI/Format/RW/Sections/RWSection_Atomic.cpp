#include "RWSection_Atomic.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Atomic::RWSection_Atomic(void) :
	m_uiFrameIndex(0),
	m_uiGeometryIndex(0),
	m_uiUnknown1(0),
	m_uiUnknown2(0)
{
	setSectionId(RW_SECTION_ATOMIC);
}

// serialization
void							RWSection_Atomic::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_uiFrameIndex = pDataReader->readUint32();
	m_uiGeometryIndex = pDataReader->readUint32();
	m_uiUnknown1 = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
}

void							RWSection_Atomic::_serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	pDataWriter->writeUint32(m_uiFrameIndex);
	pDataWriter->writeUint32(m_uiGeometryIndex);
	pDataWriter->writeUint32(m_uiUnknown1);
	pDataWriter->writeUint32(m_uiUnknown2);
}