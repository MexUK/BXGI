#include "CIDEEntry_Other.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_Other::CIDEEntry_Other(CIDEFormat *pIDEFormat) :
	CIDEEntry(pIDEFormat)
{
}

void		CIDEEntry_Other::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void		CIDEEntry_Other::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeStringRef(getLine());
}