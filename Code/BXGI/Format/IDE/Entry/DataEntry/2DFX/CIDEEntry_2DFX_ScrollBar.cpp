#include "CIDEEntry_2DFX_ScrollBar.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_ScrollBar::CIDEEntry_2DFX_ScrollBar(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_SCROLL_BAR)
{
}

void			CIDEEntry_2DFX_ScrollBar::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_ScrollBar::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}