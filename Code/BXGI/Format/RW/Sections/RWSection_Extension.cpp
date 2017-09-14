#include "RWSection_Extension.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

// serialization
void							RWSection_Extension::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	// no body data to read, contained RW sections are unserialized elsewhere
}

void							RWSection_Extension::_serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	// no body data to write
}