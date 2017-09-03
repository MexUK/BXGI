#include "IPLEntry_PATH_Group.h"
#include "Format/IPL/Entry/DataEntry/IPLEntry_PATH.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_PATH_Group::IPLEntry_PATH_Group(IPLFormat *pIPLFormat) :
	IPLEntry_PATH(pIPLFormat, IPL_PATH_GROUP),
	m_iMinusOne(0)
{
}

void			IPLEntry_PATH_Group::unload(void)
{
	for (IPLEntry *pIPLEntry : getEntries())
	{
		pIPLEntry->unload();
		delete pIPLEntry;
	}
}

void			IPLEntry_PATH_Group::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 2:
		IPLEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_strGroupType = pDataReader->readTokenString();
		m_iMinusOne = pDataReader->readTokenInt32();
		break;
	}

	setLatestPathGroup(this);
}

void			IPLEntry_PATH_Group::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		IPLEntry_PATH::serialize();

		pDataWriter->writeStringRef(m_strGroupType);
		pDataWriter->writeInt32(m_iMinusOne);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}