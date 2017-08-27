#include "CIDEEntry_2DFX_LightShaft.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_LightShaft::CIDEEntry_2DFX_LightShaft(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_LIGHT_SHAFT)
{
}

void			CIDEEntry_2DFX_LightShaft::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_LightShaft::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}