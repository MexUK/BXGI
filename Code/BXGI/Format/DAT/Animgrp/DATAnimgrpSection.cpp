#include "DATAnimgrpSection.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

DATAnimgrpSection::DATAnimgrpSection(void) :
	m_pDATAnimgrpFormat(nullptr)
{
}

void					DATAnimgrpSection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_strWalkStyleName = pDataReader->readTokenString();
	m_strIFPFileName = pDataReader->readTokenString();
	m_strAnimationType = pDataReader->readTokenString();
	uint32 uiEntryCount = pDataReader->readTokenUint32();
}

void					DATAnimgrpSection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeToken(m_strWalkStyleName);
	pDataWriter->writeToken(m_strIFPFileName);
	pDataWriter->writeToken(m_strAnimationType);
	pDataWriter->writeToken(getEntryCount());
}