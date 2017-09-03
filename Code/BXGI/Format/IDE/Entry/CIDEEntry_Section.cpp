#include "CIDEEntry_Section.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/CIDEFormat.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_Section::CIDEEntry_Section(CIDEFormat *pIDEFormat) :
	CIDEEntry(pIDEFormat)
{
}

void		CIDEEntry_Section::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	// setSectionType() has already been called, as the EIDESectionType has already been detected.
}

void		CIDEEntry_Section::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}