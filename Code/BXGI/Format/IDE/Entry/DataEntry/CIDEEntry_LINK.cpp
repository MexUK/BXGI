#include "CIDEEntry_LINK.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_LINK::CIDEEntry_LINK(CIDEFormat *pIDEFormat) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_LINK)
{
}

void			CIDEEntry_LINK::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_LINK::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}