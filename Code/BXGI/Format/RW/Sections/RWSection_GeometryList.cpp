#include "RWSection_GeometryList.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_GeometryList::RWSection_GeometryList(void) :
	m_uiGeometryCount(0)
{
	setSectionId(RW_SECTION_GEOMETRY_LIST);
}

// serialization
void							RWSection_GeometryList::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_uiGeometryCount = pDataReader->readUint32();
}

void							RWSection_GeometryList::_serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	pDataWriter->writeUint32(m_uiGeometryCount);
}