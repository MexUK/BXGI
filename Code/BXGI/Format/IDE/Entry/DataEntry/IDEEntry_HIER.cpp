#include "IDEEntry_HIER.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_HIER::IDEEntry_HIER(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_HIER),
	m_uiObjectId(0)
{
}

void			IDEEntry_HIER::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	switch (pDataReader->getLineTokens().size())
	{
	case 3:
		// GTA III, GTA VC, GTA SA
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IDEEntry_HIER::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}