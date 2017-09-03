#include "CRWSection_Extension.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

void							CRWSection_Extension::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	// no body data to read, contained RW sections are unserialized elsewhere
}

void							CRWSection_Extension::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	// no body data to write
}