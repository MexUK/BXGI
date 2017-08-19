#include "CRWSection_GeometryList.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_GeometryList::CRWSection_GeometryList(void) :
	m_uiGeometryCount(0)
{
	setSectionId(RW_SECTION_GEOMETRY_LIST);
}

void							CRWSection_GeometryList::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_uiGeometryCount = pDataReader->readUint32();
}

void							CRWSection_GeometryList::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeUint32(m_uiGeometryCount);
}