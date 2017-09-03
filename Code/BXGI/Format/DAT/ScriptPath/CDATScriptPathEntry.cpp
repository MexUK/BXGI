#include "CDATScriptPathEntry.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CDATScriptPathEntry::CDATScriptPathEntry(void)
{
	m_vecPosition.x = 0.0f;
	m_vecPosition.y = 0.0f;
	m_vecPosition.y = 0.0f;
}

void					CDATScriptPathEntry::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_vecPosition = pDataReader->readTokenVector3D();
}

void					CDATScriptPathEntry::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeToken(m_vecPosition);
}