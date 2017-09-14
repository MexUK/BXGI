#include "RWSection_Clump.h"
#include "Engine/RW/RWManager.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Clump::RWSection_Clump(void) :
	m_uiAtomicallyLinkedObjectCount(0),
	m_uiLightCount(0),
	m_uiCameraCount(0)
{
	setSectionId(RW_SECTION_CLUMP);
}

// serialization
void							RWSection_Clump::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	/*
	todo - tidy
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

	// todo - tidy
	//if ((uiRWVersionId >= 0x0C02FFFF || uiRWVersionId == 0x0400CFFF) ////////// && uiRWVersionId != 0x1003FFFF ////////)
	//if (m_uiStructSectionSize == 12)
	if (RWVersion::unpackVersionStamp(m_uiSectionRWVersion) > 0x33000)
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

void							RWSection_Clump::_serialize(void)
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