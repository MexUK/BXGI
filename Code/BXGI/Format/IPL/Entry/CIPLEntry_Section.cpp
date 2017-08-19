#include "CIPLEntry_Section.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Format/IPL/CIPLFormat.h"

using namespace bxcf;
using namespace bxgi;

void		CIPLEntry_Section::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	// setSectionType() has already been called, as the eIPLSectionType has already been detected.
}

void		CIPLEntry_Section::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}