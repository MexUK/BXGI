#include "CRWEntry_2dEffect_SunGlare.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_SunGlare::CRWEntry_2dEffect_SunGlare(void) :
	C2dEffect(_2DFX_SUN_GLARE)
{
};

void							CRWEntry_2dEffect_SunGlare::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	// this 2d effect does not have body data
}

void							CRWEntry_2dEffect_SunGlare::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	// this 2d effect does not have body data
}