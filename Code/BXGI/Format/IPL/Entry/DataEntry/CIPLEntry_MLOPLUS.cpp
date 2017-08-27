#include "CIPLEntry_MLOPLUS.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_MLOPLUS::CIPLEntry_MLOPLUS(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_MLOPLUS)
{
}

void			CIPLEntry_MLOPLUS::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIPLEntry_MLOPLUS::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}