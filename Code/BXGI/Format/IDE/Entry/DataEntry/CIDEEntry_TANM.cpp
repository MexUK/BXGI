#include "CIDEEntry_TANM.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_TANM::CIDEEntry_TANM(CIDEFormat *pIDEFormat) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_TANM)
{
}

void			CIDEEntry_TANM::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_TANM::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}