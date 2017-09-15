#include "IDEEntry_2DFX_WalkDontWalkAttr.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_WalkDontWalkAttr::IDEEntry_2DFX_WalkDontWalkAttr(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_WALK_DONT_WALK_ATTR)
{
}

void			IDEEntry_2DFX_WalkDontWalkAttr::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_2DFX_WalkDontWalkAttr::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}