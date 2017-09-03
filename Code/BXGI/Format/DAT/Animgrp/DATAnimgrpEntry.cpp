#include "DATAnimgrpEntry.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

DATAnimgrpEntry::DATAnimgrpEntry(void) :
	m_pDATAnimgrpSection(nullptr)
{
}

void					DATAnimgrpEntry::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_strAnimationName = pDataReader->readTokenString();
}

void					DATAnimgrpEntry::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeToken(m_strAnimationName);
}