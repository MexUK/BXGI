#include "IDEEntry_UnknownSection.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_UnknownSection::IDEEntry_UnknownSection(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_UNKNOWN)
{
}

void			IDEEntry_UnknownSection::unserialize(void)
{
	setLine(*m_pFormat->m_reader.getActiveLine());
}

void			IDEEntry_UnknownSection::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeStringRef(m_strLine);
}