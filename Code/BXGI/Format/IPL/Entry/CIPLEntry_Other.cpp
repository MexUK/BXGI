#include "CIPLEntry_Other.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_Other::CIPLEntry_Other(CIPLFormat *pIPLFormat) :
	CIPLEntry(pIPLFormat)
{
}

void		CIPLEntry_Other::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void		CIPLEntry_Other::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(getLine());
}