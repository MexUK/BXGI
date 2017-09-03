#include "DATAnimgrpFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxgi;

DATAnimgrpFormat::DATAnimgrpFormat(void) :
	Format(false, bxcf::LITTLE_ENDIAN)
{
}

void					DATAnimgrpFormat::unserialize(void)
{
	// todo DataReader::get()->readSectionLineEntries<eDATAnimgrpLineType, DATAnimgrpSection, DATAnimgrpEntry>(this);
}

void					DATAnimgrpFormat::serialize(void)
{
	// todo DataWriter::get()->writeSectionLineEntries<DATAnimgrpSection, DATAnimgrpEntry>(this);
}