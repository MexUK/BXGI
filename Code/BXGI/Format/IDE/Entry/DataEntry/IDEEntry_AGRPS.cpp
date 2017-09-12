#include "IDEEntry_AGRPS.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_AGRPS::IDEEntry_AGRPS(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_AGRPS)
{
}

void			IDEEntry_AGRPS::unserialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_AGRPS::serialize(void)
{
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}