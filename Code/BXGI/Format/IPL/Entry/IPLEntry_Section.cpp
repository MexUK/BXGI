#include "IPLEntry_Section.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_Section::IPLEntry_Section(IPLFormat *pIPLFormat) :
	IPLEntry(pIPLFormat)
{
}

void		IPLEntry_Section::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	// setSectionType() has already been called, as the EIPLSectionType has already been detected.
}

void		IPLEntry_Section::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}