#include "RWSection_Clump.h"
#include "Engine/RW/RWManager.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Clump::RWSection_Clump(void) :
	m_uiAtomicallyLinkedObjectCount(0),
	m_uiLightCount(0),
	m_uiCameraCount(0)
{
	setSectionId(RW_SECTION_CLUMP);
}

void							RWSection_Clump::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	/*
	todo
	if (m_pParentNode == pDFFFile) // found first clump struct in the DFF file
	{
		setSectionHeaderSkipped(false);
		pLatestClump = pClump;
		m_pParentNode = pClump;
	}
	else
	{
		setSectionHeaderSkipped(true);
	}
	*/

	m_uiAtomicallyLinkedObjectCount = pDataReader->readUint32();

	//if ((uiRWVersionId >= 0x0C02FFFF || uiRWVersionId == 0x0400CFFF) ////////// && uiRWVersionId != 0x1003FFFF ////////)
	if (m_uiStructSectionSize == 12)
	{
		m_uiLightCount = pDataReader->readUint32();
		m_uiCameraCount = pDataReader->readUint32();
	}
	else
	{
		m_uiLightCount = 1;
		m_uiCameraCount = 1;
	}
}

void							RWSection_Clump::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiAtomicallyLinkedObjectCount);

	uint32 uiRWVersionCC = RWManager::get()->getSerializationRWVersion();
	if ((uiRWVersionCC >= 0x0C02FFFF || uiRWVersionCC == 0x0400CFFF) && uiRWVersionCC != 0x1003FFFF)
	{
		pDataWriter->writeUint32(m_uiLightCount);
		pDataWriter->writeUint32(m_uiCameraCount);
	}
}