#include "CDATAnimgrpEntry.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CDATAnimgrpEntry::CDATAnimgrpEntry(void) :
	m_pDATAnimgrpSection(nullptr)
{
}

void					CDATAnimgrpEntry::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_strAnimationName = pDataReader->readTokenString();
}

void					CDATAnimgrpEntry::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeToken(m_strAnimationName);
}