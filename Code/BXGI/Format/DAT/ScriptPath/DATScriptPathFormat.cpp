#include "DATScriptPathFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxgi;

DATScriptPathFormat::DATScriptPathFormat(void) :
	Format(false, bxcf::LITTLE_ENDIAN)
{
}

void					DATScriptPathFormat::unserialize(void)
{
	// todo DataReader::get()->unserializeLineEntries<DATScriptPathEntry>(this);
}

void					DATScriptPathFormat::serialize(void)
{
	// todo DataWriter::get()->serializeLineEntries<DATScriptPathEntry>(this);
}