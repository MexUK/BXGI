#include "CIPLEntry_UnknownSection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_UnknownSection::CIPLEntry_UnknownSection(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_UNKNOWN)
{
}

void			CIPLEntry_UnknownSection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	setLine(*pDataReader->getActiveLine());
}

void			CIPLEntry_UnknownSection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeStringRef(m_strLine);
}