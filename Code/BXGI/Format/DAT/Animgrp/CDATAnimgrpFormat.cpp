#include "CDATAnimgrpFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxgi;

CDATAnimgrpFormat::CDATAnimgrpFormat(void) :
	Format(false, bxcf::LITTLE_ENDIAN)
{
}

void					CDATAnimgrpFormat::unserialize(void)
{
	// todo DataReader::get()->readSectionLineEntries<eDATAnimgrpLineType, CDATAnimgrpSection, CDATAnimgrpEntry>(this);
}

void					CDATAnimgrpFormat::serialize(void)
{
	// todo DataWriter::get()->writeSectionLineEntries<CDATAnimgrpSection, CDATAnimgrpEntry>(this);
}