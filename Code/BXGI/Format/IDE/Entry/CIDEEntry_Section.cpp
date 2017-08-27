#include "CIDEEntry_Section.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Format/IDE/CIDEFormat.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_Section::CIDEEntry_Section(CIDEFormat *pIDEFormat) :
	CIDEEntry(pIDEFormat)
{
}

void		CIDEEntry_Section::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	// setSectionType() has already been called, as the EIDESectionType has already been detected.
}

void		CIDEEntry_Section::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}