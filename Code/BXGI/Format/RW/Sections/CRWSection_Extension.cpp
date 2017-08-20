#include "CRWSection_Extension.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void							CRWSection_Extension::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	// no body data to read, contained RW sections are unserialized elsewhere
}

void							CRWSection_Extension::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	// no body data to write
}