#include "CIPLEntry_Section.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/CIPLFormat.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_Section::CIPLEntry_Section(CIPLFormat *pIPLFormat) :
	CIPLEntry(pIPLFormat)
{
}

void		CIPLEntry_Section::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	// setSectionType() has already been called, as the EIPLSectionType has already been detected.
}

void		CIPLEntry_Section::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}