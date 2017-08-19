#include "CIDEEntry_2DFX_Escalator.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIDEEntry_2DFX_Escalator::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_Escalator::serialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}