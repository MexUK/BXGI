#include "CIDEEntry_2DFX_Audio.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_Audio::CIDEEntry_2DFX_Audio(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_AUDIO)
{
}

void			CIDEEntry_2DFX_Audio::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_Audio::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}