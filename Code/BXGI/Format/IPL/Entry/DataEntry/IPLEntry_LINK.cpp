#include "IPLEntry_LINK.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_LINK::IPLEntry_LINK(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_LINK)
{
}

void			IPLEntry_LINK::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_LINK::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}