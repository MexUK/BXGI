#include "CRWSection_UnknownSection.h"
#include "Engine/RW/CRWManager.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_UnknownSection::CRWSection_UnknownSection(void) :
	m_uiActualSectionId(0)
{
	setSectionId(RW_SECTION_UNKNOWN);
	setUnknownSection(true);
}

void							CRWSection_UnknownSection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_uiActualSectionId = m_eSectionId;
	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							CRWSection_UnknownSection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	uint32 uiRWVersionCC = CRWManager::get()->getRWVersionCCForSerialization();

	/*
	todo
	pDataWriter->writeUint32(m_uiActualSectionId);
	pDataWriter->writeUint32(m_strData.length());
	pDataWriter->writeUint32(uiRWVersionCC);
	*/

	pDataWriter->writeStringRef(m_strData);
}