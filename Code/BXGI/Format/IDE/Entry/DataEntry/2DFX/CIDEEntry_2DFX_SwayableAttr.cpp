#include "CIDEEntry_2DFX_SwayableAttr.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_SwayableAttr::CIDEEntry_2DFX_SwayableAttr(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_SWAYABLE_ATTR)
{
}

void			CIDEEntry_2DFX_SwayableAttr::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_SwayableAttr::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}