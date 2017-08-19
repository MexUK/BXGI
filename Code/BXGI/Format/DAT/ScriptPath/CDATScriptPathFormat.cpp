#include "CDATScriptPathFormat.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace std;
using namespace bxgi;

CDATScriptPathFormat::CDATScriptPathFormat(void) :
	CFormat(false, bxcf::LITTLE_ENDIAN)
{
}

void					CDATScriptPathFormat::unserialize(void)
{
	// todo CDataReader::get()->unserializeLineEntries<CDATScriptPathEntry>(this);
}

void					CDATScriptPathFormat::serialize(void)
{
	// todo CDataWriter::get()->serializeLineEntries<CDATScriptPathEntry>(this);
}