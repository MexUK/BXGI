#include "CDATScriptPathFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxgi;

CDATScriptPathFormat::CDATScriptPathFormat(void) :
	Format(false, bxcf::LITTLE_ENDIAN)
{
}

void					CDATScriptPathFormat::unserialize(void)
{
	// todo DataReader::get()->unserializeLineEntries<CDATScriptPathEntry>(this);
}

void					CDATScriptPathFormat::serialize(void)
{
	// todo DataWriter::get()->serializeLineEntries<CDATScriptPathEntry>(this);
}