#include "CDATAnimgrpSection.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CDATAnimgrpSection::CDATAnimgrpSection(void) :
	m_pDATAnimgrpFormat(nullptr)
{
}

void					CDATAnimgrpSection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_strWalkStyleName = pDataReader->readTokenString();
	m_strIFPFileName = pDataReader->readTokenString();
	m_strAnimationType = pDataReader->readTokenString();
	uint32 uiEntryCount = pDataReader->readTokenUint32();
}

void					CDATAnimgrpSection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->writeToken(m_strWalkStyleName);
	pDataWriter->writeToken(m_strIFPFileName);
	pDataWriter->writeToken(m_strAnimationType);
	pDataWriter->writeToken(getEntryCount());
}