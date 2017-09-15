#include "IDEEntry_TXDP.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_TXDP::IDEEntry_TXDP(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_TXDP)
{
}

void			IDEEntry_TXDP::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	switch (pDataReader->getLineTokens().size())
	{
	case 2:
		// GTA SA, GTA IV
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA | GAME_FLAG_GTA_IV);
		m_strTXDName = pDataReader->readTokenString();
		m_strParentTXDName = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IDEEntry_TXDP::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strParentTXDName);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}