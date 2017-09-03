#include "IDEEntry_PATH.h"
#include "Format/IDE/Entry/DataEntry/PATH/IDEEntry_PATH_Group.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_PATH_Group*				IDEEntry_PATH::m_pLatestPathGroup = nullptr;

IDEEntry_PATH::IDEEntry_PATH(IDEFormat *pIDEFormat, EIDEPathType EIDEPathTypeValue) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_PATH),
	m_EIDEPathType(EIDEPathTypeValue)
{
}

void			IDEEntry_PATH::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
}

void			IDEEntry_PATH::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
}