#include "RWEntry_2dEffect_SunGlare.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_SunGlare::RWEntry_2dEffect_SunGlare(void) :
	_2dEffect(_2DFX_SUN_GLARE)
{
};

void							RWEntry_2dEffect_SunGlare::unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;
	
	// this 2d effect does not have body data
}

void							RWEntry_2dEffect_SunGlare::serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;
	
	// this 2d effect does not have body data
}