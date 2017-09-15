#include "IPLEntry_LODM.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_LODM::IPLEntry_LODM(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_LODM)
{
}

void			IPLEntry_LODM::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_LODM::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}