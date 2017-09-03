#include "CIDEEntry_2DFX_SunReflection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_SunReflection::CIDEEntry_2DFX_SunReflection(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_SUN_REFLECTION)
{
}

void			CIDEEntry_2DFX_SunReflection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	CIDEEntry_2DFX::unserialize();
}

void			CIDEEntry_2DFX_SunReflection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	CIDEEntry_2DFX::serialize();
}