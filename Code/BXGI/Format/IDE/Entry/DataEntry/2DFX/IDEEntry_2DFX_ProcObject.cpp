#include "IDEEntry_2DFX_ProcObject.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_ProcObject::IDEEntry_2DFX_ProcObject(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_PROC_OBJECT)
{
}

void			IDEEntry_2DFX_ProcObject::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_2DFX_ProcObject::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}