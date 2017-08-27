#include "CIPLEntry_LINK.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_LINK::CIPLEntry_LINK(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_LINK)
{
}

void			CIPLEntry_LINK::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIPLEntry_LINK::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}