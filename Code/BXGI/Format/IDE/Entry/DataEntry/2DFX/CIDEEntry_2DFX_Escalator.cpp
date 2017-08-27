#include "CIDEEntry_2DFX_Escalator.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_Escalator::CIDEEntry_2DFX_Escalator(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_ESCALATOR)
{
}

void			CIDEEntry_2DFX_Escalator::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_Escalator::serialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}