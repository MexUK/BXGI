#include "IPLEntry_VNOD.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_VNOD::IPLEntry_VNOD(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_VNOD)
{
}

void			IPLEntry_VNOD::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_VNOD::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}