#include "CIPLEntry_2DFX.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_2DFX::CIPLEntry_2DFX(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_2DFX)
{
}

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