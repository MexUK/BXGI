#include "IDEEntry_2DFX_Escalator.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Escalator::IDEEntry_2DFX_Escalator(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_ESCALATOR)
{
}

void			IDEEntry_2DFX_Escalator::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_2DFX_Escalator::serialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}