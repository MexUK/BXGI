#include "CRWSection_MaterialList.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_MaterialList::CRWSection_MaterialList(void) :
	m_uiMaterialCount(0)
{
	setSectionId(RW_SECTION_MATERIAL_LIST);
}

void							CRWSection_MaterialList::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_uiMaterialCount = pDataReader->readUint32();
	m_strReservedValues = pDataReader->readString(4 * m_uiMaterialCount);
}

void							CRWSection_MaterialList::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeUint32(m_uiMaterialCount);
	pDataWriter->writeStringRef(m_strReservedValues);
}