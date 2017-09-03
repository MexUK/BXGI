#include "CIDEEntry_2DFX_WorldPoint.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_WorldPoint::CIDEEntry_2DFX_WorldPoint(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_WORLD_POINT)
{
}

void			CIDEEntry_2DFX_WorldPoint::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_WorldPoint::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}