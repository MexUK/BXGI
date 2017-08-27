#include "CIPLEntry_Other.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_Other::CIPLEntry_Other(CIPLFormat *pIPLFormat) :
	CIPLEntry(pIPLFormat)
{
}

void		CIPLEntry_Other::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void		CIPLEntry_Other::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeStringRef(getLine());
}