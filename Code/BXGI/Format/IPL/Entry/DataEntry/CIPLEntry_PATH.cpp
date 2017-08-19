#include "CIPLEntry_PATH.h"
#include "Format/IPL/Entry/DataEntry/PATH/CIPLEntry_PATH_Group.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_PATH_Group*	CIPLEntry_PATH::m_pLatestPathGroup = nullptr;

void			CIPLEntry_PATH::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
}

void			CIPLEntry_PATH::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
}