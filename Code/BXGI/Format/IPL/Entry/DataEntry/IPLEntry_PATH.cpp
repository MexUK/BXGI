#include "IPLEntry_PATH.h"
#include "Format/IPL/Entry/DataEntry/PATH/IPLEntry_PATH_Group.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_PATH_Group*	IPLEntry_PATH::m_pLatestPathGroup = nullptr;

IPLEntry_PATH::IPLEntry_PATH(IPLFormat *pIPLFormat, EIPLPathType EIPLPathTypeValue) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_PATH),
	m_uiIPLPathType(EIPLPathTypeValue)
{
}

void			IPLEntry_PATH::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
}

void			IPLEntry_PATH::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;
}