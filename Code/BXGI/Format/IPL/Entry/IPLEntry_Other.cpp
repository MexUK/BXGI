#include "IPLEntry_Other.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_Other::IPLEntry_Other(IPLFormat *pIPLFormat) :
	IPLEntry(pIPLFormat)
{
}

void		IPLEntry_Other::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void		IPLEntry_Other::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(getLine());
}