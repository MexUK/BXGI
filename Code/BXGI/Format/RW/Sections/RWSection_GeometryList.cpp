#include "RWSection_GeometryList.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWSection_GeometryList::RWSection_GeometryList(void) :
	m_uiGeometryCount(0)
{
	setSectionId(RW_SECTION_GEOMETRY_LIST);
}

void							RWSection_GeometryList::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_uiGeometryCount = pDataReader->readUint32();
}

void							RWSection_GeometryList::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiGeometryCount);
}