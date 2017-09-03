#include "IPLEntry_MZON.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_MZON::IPLEntry_MZON(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_MZON)
{
}

void			IPLEntry_MZON::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	switch (uiLineTokenCount)
	{
	case 10:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_IV);

		m_strZoneName = pDataReader->readTokenString();
		m_uiZoneType = pDataReader->readTokenUint32();
		m_vecBottomLeftPosition = pDataReader->readTokenVector3D();
		m_vecTopRightPosition = pDataReader->readTokenVector3D();
		m_uiLevel = pDataReader->readTokenUint32();
		m_strZoneText = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_MZON::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiZoneType);
		pDataWriter->writeToken(m_vecBottomLeftPosition);
		pDataWriter->writeToken(m_vecTopRightPosition);
		pDataWriter->writeToken(m_uiLevel);
		pDataWriter->writeToken(m_strZoneText);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}