#include "IDEEntry_2DFX_SunReflection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_SunReflection::IDEEntry_2DFX_SunReflection(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_SUN_REFLECTION)
{
}

void			IDEEntry_2DFX_SunReflection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	IDEEntry_2DFX::unserialize();
}

void			IDEEntry_2DFX_SunReflection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	IDEEntry_2DFX::serialize();
}