#include "IPLEntry_2DFX.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_2DFX::IPLEntry_2DFX(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_2DFX)
{
}

void			IPLEntry_2DFX::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IPLEntry_2DFX::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}