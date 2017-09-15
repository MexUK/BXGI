#include "IDEEntry_Section.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_Section::IDEEntry_Section(IDEFormat *pIDEFormat) :
	IDEEntry(pIDEFormat)
{
}

void		IDEEntry_Section::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	// setSectionType() has already been called, as the EIDESectionType has already been detected.
}

void		IDEEntry_Section::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}