#include "IDEEntry_2DFX_Escalator.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Escalator::IDEEntry_2DFX_Escalator(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_ESCALATOR)
{
}

void			IDEEntry_2DFX_Escalator::unserialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_2DFX_Escalator::serialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}