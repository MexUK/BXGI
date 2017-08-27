#include "CIPLEntry_PATH_Group.h"
#include "Format/IPL/Entry/DataEntry/CIPLEntry_PATH.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_PATH_Group::CIPLEntry_PATH_Group(CIPLFormat *pIPLFormat) :
	CIPLEntry_PATH(pIPLFormat, IPL_PATH_GROUP),
	m_iMinusOne(0)
{
}

void			CIPLEntry_PATH_Group::unload(void)
{
	for (CIPLEntry *pIPLEntry : getEntries())
	{
		pIPLEntry->unload();
		delete pIPLEntry;
	}
}

void			CIPLEntry_PATH_Group::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 2:
		CIPLEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_strGroupType = pDataReader->readTokenString();
		m_iMinusOne = pDataReader->readTokenInt32();
		break;
	}

	setLatestPathGroup(this);
}

void			CIPLEntry_PATH_Group::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	switch (getFormatType())
	{
	case 0:
		CIPLEntry_PATH::serialize();

		pDataWriter->writeStringRef(m_strGroupType);
		pDataWriter->writeInt32(m_iMinusOne);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}