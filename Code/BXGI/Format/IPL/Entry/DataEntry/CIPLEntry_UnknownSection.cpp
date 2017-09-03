#include "CIPLEntry_UnknownSection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_UnknownSection::CIPLEntry_UnknownSection(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_UNKNOWN)
{
}

void			CIPLEntry_UnknownSection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void			CIPLEntry_UnknownSection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strLine);
}