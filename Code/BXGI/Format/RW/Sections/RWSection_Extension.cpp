#include "RWSection_Extension.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

void							RWSection_Extension::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	// no body data to read, contained RW sections are unserialized elsewhere
}

void							RWSection_Extension::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	// no body data to write
}