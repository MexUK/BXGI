#include "CIDEEntry_TREE.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_TREE::CIDEEntry_TREE(CIDEFormat *pIDEFormat) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_TREE)
{
}

void			CIDEEntry_TREE::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_TREE::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}