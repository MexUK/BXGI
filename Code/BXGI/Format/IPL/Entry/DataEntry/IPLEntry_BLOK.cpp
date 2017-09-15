#include "IPLEntry_BLOK.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_BLOK::IPLEntry_BLOK(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_BLOK)
{
}

void			IPLEntry_BLOK::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_BLOK::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}