#include "IPLEntry_MULT.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_MULT::IPLEntry_MULT(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_MULT)
{
}

void			IPLEntry_MULT::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_MULT::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}