#include "CDATAnimgrpEntry.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CDATAnimgrpEntry::CDATAnimgrpEntry(void) :
	m_pDATAnimgrpSection(nullptr)
{
}

void					CDATAnimgrpEntry::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_strAnimationName = pDataReader->readTokenString();
}

void					CDATAnimgrpEntry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->writeToken(m_strAnimationName);
}