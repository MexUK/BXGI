#include "IDEEntry_2DFX_SunReflection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_SunReflection::IDEEntry_2DFX_SunReflection(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_SUN_REFLECTION)
{
}

void			IDEEntry_2DFX_SunReflection::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	IDEEntry_2DFX::unserialize();
}

void			IDEEntry_2DFX_SunReflection::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	IDEEntry_2DFX::serialize();
}