#include "IDEEntry_2DFX_WorldPoint.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_WorldPoint::IDEEntry_2DFX_WorldPoint(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_WORLD_POINT)
{
}

void			IDEEntry_2DFX_WorldPoint::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_2DFX_WorldPoint::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}