#include "RWSection_UnknownSection.h"
#include "Engine/RW/RWManager.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWSection_UnknownSection::RWSection_UnknownSection(void) :
	m_uiActualSectionId(0)
{
	setSectionId(RW_SECTION_UNKNOWN);
	setUnknownSection(true);
}

void							RWSection_UnknownSection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_uiActualSectionId = m_uiSectionId;
	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							RWSection_UnknownSection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	uint32 uiRWVersionCC = RWManager::get()->getSerializationRWVersion();

	/*
	todo
	pDataWriter->writeUint32(m_uiActualSectionId);
	pDataWriter->writeUint32(m_strData.length());
	pDataWriter->writeUint32(uiRWVersionCC);
	*/

	pDataWriter->writeStringRef(m_strData);
}