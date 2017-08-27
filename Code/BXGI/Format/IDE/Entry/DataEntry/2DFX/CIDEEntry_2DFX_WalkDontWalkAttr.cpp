#include "CIDEEntry_2DFX_WalkDontWalkAttr.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_WalkDontWalkAttr::CIDEEntry_2DFX_WalkDontWalkAttr(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_WALK_DONT_WALK_ATTR)
{
}

void			CIDEEntry_2DFX_WalkDontWalkAttr::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_WalkDontWalkAttr::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}