#include "CIDEEntry_2DFX_LightShaft.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_LightShaft::CIDEEntry_2DFX_LightShaft(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_LIGHT_SHAFT)
{
}

void			CIDEEntry_2DFX_LightShaft::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_LightShaft::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}