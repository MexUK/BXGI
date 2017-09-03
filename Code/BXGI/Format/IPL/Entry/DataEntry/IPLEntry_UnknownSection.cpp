#include "IPLEntry_UnknownSection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_UnknownSection::IPLEntry_UnknownSection(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_UNKNOWN)
{
}

void			IPLEntry_UnknownSection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void			IPLEntry_UnknownSection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strLine);
}