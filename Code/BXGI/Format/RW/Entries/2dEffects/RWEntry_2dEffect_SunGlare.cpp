#include "RWEntry_2dEffect_SunGlare.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_SunGlare::RWEntry_2dEffect_SunGlare(void) :
	_2dEffect(_2DFX_SUN_GLARE)
{
};

void							RWEntry_2dEffect_SunGlare::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	// this 2d effect does not have body data
}

void							RWEntry_2dEffect_SunGlare::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	// this 2d effect does not have body data
}