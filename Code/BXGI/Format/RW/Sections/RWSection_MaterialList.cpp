#include "RWSection_MaterialList.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_MaterialList::RWSection_MaterialList(void) :
	m_uiMaterialCount(0)
{
	setSectionId(RW_SECTION_MATERIAL_LIST);
}

// serialization
void							RWSection_MaterialList::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_uiMaterialCount = pDataReader->readUint32();
	m_strReservedValues = pDataReader->readString(4 * m_uiMaterialCount);
}

void							RWSection_MaterialList::_serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiMaterialCount);
	pDataWriter->writeStringRef(m_strReservedValues);
}