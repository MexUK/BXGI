#include "CRWEntry_2dEffect_SunGlare.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_SunGlare::CRWEntry_2dEffect_SunGlare(void) :
	C2dEffect(_2DFX_SUN_GLARE)
{
};

void							CRWEntry_2dEffect_SunGlare::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	// this 2d effect does not have body data
}

void							CRWEntry_2dEffect_SunGlare::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	// this 2d effect does not have body data
}