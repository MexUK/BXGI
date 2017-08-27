#include "CIDEEntry_2DFX_SunReflection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_SunReflection::CIDEEntry_2DFX_SunReflection(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_SUN_REFLECTION)
{
}

void			CIDEEntry_2DFX_SunReflection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	CIDEEntry_2DFX::unserialize();
}

void			CIDEEntry_2DFX_SunReflection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	CIDEEntry_2DFX::serialize();
}