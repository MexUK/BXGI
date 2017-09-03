#include "CIDEEntry_Other.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_Other::CIDEEntry_Other(CIDEFormat *pIDEFormat) :
	CIDEEntry(pIDEFormat)
{
}

void		CIDEEntry_Other::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void		CIDEEntry_Other::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(getLine());
}