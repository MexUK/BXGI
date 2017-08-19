#include "CDATAnimgrpFormat.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace std;
using namespace bxgi;

CDATAnimgrpFormat::CDATAnimgrpFormat(void) :
	CFormat(false, bxcf::LITTLE_ENDIAN)
{
}

void					CDATAnimgrpFormat::unserialize(void)
{
	// todo CDataReader::get()->readSectionLineEntries<eDATAnimgrpLineType, CDATAnimgrpSection, CDATAnimgrpEntry>(this);
}

void					CDATAnimgrpFormat::serialize(void)
{
	// todo CDataWriter::get()->writeSectionLineEntries<CDATAnimgrpSection, CDATAnimgrpEntry>(this);
}