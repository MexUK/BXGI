#include "CIDEEntry_TXDP.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_TXDP::CIDEEntry_TXDP(CIDEFormat *pIDEFormat) :
	CIDEEntry_Data(pIDEFormat, IDE_SECTION_TXDP)
{
}

void			CIDEEntry_TXDP::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

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

void			CIDEEntry_TXDP::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

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