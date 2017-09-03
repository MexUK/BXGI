#include "CIDEEntry_UnknownSection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_UnknownSection::CIDEEntry_UnknownSection(CIDEFormat *pIDEFormat) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_UNKNOWN)
{
}

void			CIDEEntry_UnknownSection::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	setLine(*pDataReader->getActiveLine());
}

void			CIDEEntry_UnknownSection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strLine);
}