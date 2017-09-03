#include "CIDEEntry_PATH.h"
#include "Format/IDE/Entry/DataEntry/PATH/CIDEEntry_PATH_Group.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_PATH_Group*				CIDEEntry_PATH::m_pLatestPathGroup = nullptr;

CIDEEntry_PATH::CIDEEntry_PATH(CIDEFormat *pIDEFormat, EIDEPathType EIDEPathTypeValue) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_PATH),
	m_EIDEPathType(EIDEPathTypeValue)
{
}

void			CIDEEntry_PATH::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
}

void			CIDEEntry_PATH::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
}