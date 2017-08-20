#include "CRWSection_Atomic.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_Atomic::CRWSection_Atomic(void) :
	m_uiFrameIndex(0),
	m_uiGeometryIndex(0),
	m_uiUnknown1(0),
	m_uiUnknown2(0)
{
	setSectionId(RW_SECTION_ATOMIC);
}

void							CRWSection_Atomic::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_uiFrameIndex = pDataReader->readUint32();
	m_uiGeometryIndex = pDataReader->readUint32();
	m_uiUnknown1 = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
}

void							CRWSection_Atomic::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeUint32(m_uiFrameIndex);
	pDataWriter->writeUint32(m_uiGeometryIndex);
	pDataWriter->writeUint32(m_uiUnknown1);
	pDataWriter->writeUint32(m_uiUnknown2);
}