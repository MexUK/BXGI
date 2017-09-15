#include "IPLEntry_UnknownSection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_UnknownSection::IPLEntry_UnknownSection(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_UNKNOWN)
{
}

void			IPLEntry_UnknownSection::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	setLine(*pDataReader->getActiveLine());
}

void			IPLEntry_UnknownSection::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	pDataWriter->writeStringRef(m_strLine);
}