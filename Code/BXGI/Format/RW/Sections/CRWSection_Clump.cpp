#include "CRWSection_Clump.h"
#include "Engine/RW/CRWManager.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_Clump::CRWSection_Clump(void) :
	m_uiAtomicallyLinkedObjectCount(0),
	m_uiLightCount(0),
	m_uiCameraCount(0)
{
	setSectionId(RW_SECTION_CLUMP);
}

void							CRWSection_Clump::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

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

void							CRWSection_Clump::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeUint32(m_uiAtomicallyLinkedObjectCount);

	uint32 uiRWVersionCC = CRWManager::get()->getRWVersionCCForSerialization();
	if ((uiRWVersionCC >= 0x0C02FFFF || uiRWVersionCC == 0x0400CFFF) && uiRWVersionCC != 0x1003FFFF)
	{
		pDataWriter->writeUint32(m_uiLightCount);
		pDataWriter->writeUint32(m_uiCameraCount);
	}
}