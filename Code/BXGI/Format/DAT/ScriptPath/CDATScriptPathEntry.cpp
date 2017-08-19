#include "CDATScriptPathEntry.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

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
	CDataReader *pDataReader = CDataReader::get();
	
	m_vecPosition = pDataReader->readTokenVector3D();
}

void					CDATScriptPathEntry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->writeToken(m_vecPosition);
}