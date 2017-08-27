#include "CIPLEntry_MULT.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_MULT::CIPLEntry_MULT(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_MULT)
{
}

void			CIPLEntry_MULT::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIPLEntry_MULT::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}