#include "IPLEntry_MLOPLUS.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_MLOPLUS::IPLEntry_MLOPLUS(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_MLOPLUS)
{
}

void			IPLEntry_MLOPLUS::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_MLOPLUS::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}