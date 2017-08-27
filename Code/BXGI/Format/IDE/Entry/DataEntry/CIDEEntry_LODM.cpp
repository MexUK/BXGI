#include "CIDEEntry_LODM.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_LODM::CIDEEntry_LODM(CIDEFormat *pIDEFormat) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_LODM)
{
}

void			CIDEEntry_LODM::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_LODM::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}