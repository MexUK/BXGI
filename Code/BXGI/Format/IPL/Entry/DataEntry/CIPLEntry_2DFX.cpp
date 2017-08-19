#include "CIPLEntry_2DFX.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIPLEntry_2DFX::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIPLEntry_2DFX::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}