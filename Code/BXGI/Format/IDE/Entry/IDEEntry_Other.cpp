#include "IDEEntry_Other.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_Other::IDEEntry_Other(IDEFormat *pIDEFormat) :
	IDEEntry(pIDEFormat)
{
}

void		IDEEntry_Other::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void		IDEEntry_Other::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(getLine());
}