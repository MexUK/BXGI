#include "CIDEEntry_PATH.h"
#include "Format/IDE/Entry/DataEntry/PATH/CIDEEntry_PATH_Group.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_PATH_Group*				CIDEEntry_PATH::m_pLatestPathGroup = nullptr;

void			CIDEEntry_PATH::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
}

void			CIDEEntry_PATH::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
}