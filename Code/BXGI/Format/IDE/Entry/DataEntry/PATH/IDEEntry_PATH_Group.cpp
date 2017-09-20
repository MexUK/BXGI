#include "IDEEntry_PATH_Group.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_PATH_Group::IDEEntry_PATH_Group(IDEFormat *pIDEFormat) :
	IDEEntry_PATH(pIDEFormat, IDE_PATH_GROUP),
	m_uiObjectId(0)
{
}

void			IDEEntry_PATH_Group::unload(void)
{
	for (IDEEntry *pIDEEntry : getEntries())
	{
		pIDEEntry->unload();
		// todo - delete pIDEEntry;
	}
}

void			IDEEntry_PATH_Group::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	switch (pDataReader->getLineTokens().size())
	{
	case 3:
		IDEEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

		m_strGroupType = pDataReader->readTokenString();
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		break;
	}

	setLatestPathGroup(this);
}

void			IDEEntry_PATH_Group::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		IDEEntry_PATH::serialize();

		pDataWriter->writeToken(m_strGroupType);
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		break;
	}
}