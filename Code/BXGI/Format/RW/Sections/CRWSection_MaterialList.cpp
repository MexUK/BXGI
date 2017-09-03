#include "CRWSection_MaterialList.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_MaterialList::CRWSection_MaterialList(void) :
	m_uiMaterialCount(0)
{
	setSectionId(RW_SECTION_MATERIAL_LIST);
}

void							CRWSection_MaterialList::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_uiMaterialCount = pDataReader->readUint32();
	m_strReservedValues = pDataReader->readString(4 * m_uiMaterialCount);
}

void							CRWSection_MaterialList::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiMaterialCount);
	pDataWriter->writeStringRef(m_strReservedValues);
}