#include "CIDEEntry_Section.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Format/IDE/CIDEFormat.h"

using namespace bxcf;
using namespace bxgi;

void		CIDEEntry_Section::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	// setSectionType() has already been called, as the eIDESectionType has already been detected.
}

void		CIDEEntry_Section::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}