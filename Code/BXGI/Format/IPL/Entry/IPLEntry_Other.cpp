#include "IPLEntry_Other.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_Other::IPLEntry_Other(IPLFormat *pIPLFormat) :
	IPLEntry(pIPLFormat)
{
}

void		IPLEntry_Other::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	setLine(*pDataReader->getActiveLine());
}

void		IPLEntry_Other::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	pDataWriter->writeStringRef(getLine());
}