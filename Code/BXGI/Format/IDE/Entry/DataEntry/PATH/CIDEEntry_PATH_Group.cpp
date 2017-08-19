#include "CIDEEntry_PATH_Group.h"
#include "Format/IDE/Entry/DataEntry/CIDEEntry_PATH.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIDEEntry_PATH_Group::unload(void)
{
	for (CIDEEntry *pIDEEntry : getEntries())
	{
		pIDEEntry->unload();
		delete pIDEEntry;
	}
}

void			CIDEEntry_PATH_Group::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 3:
		CIDEEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

		m_strGroupType = pDataReader->readTokenString();
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		break;
	}

	setLatestPathGroup(this);
}

void			CIDEEntry_PATH_Group::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	switch (getFormatType())
	{
	case 0:
		CIDEEntry_PATH::serialize();

		pDataWriter->writeToken(m_strGroupType);
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		break;
	}
}