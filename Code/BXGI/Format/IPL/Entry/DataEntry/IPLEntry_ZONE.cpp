#include "IPLEntry_ZONE.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_ZONE::IPLEntry_ZONE(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_ZONE),
	m_uiZoneType(0),
	m_vecBottomLeftPosition{ 0.0f, 0.0f, 0.0f },
	m_vecTopRightPosition{ 0.0f, 0.0f, 0.0f },
	m_uiLevel(0)
{
}

void			IPLEntry_ZONE::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	switch (uiLineTokenCount)
	{
	case 9:
	case 10:
		setFormatType(uiLineTokenCount == 9 ? 0 : 1);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

		m_strZoneName = pDataReader->readTokenString();
		m_uiZoneType = pDataReader->readTokenUint32();
		m_vecBottomLeftPosition = pDataReader->readTokenVector3D();
		m_vecTopRightPosition = pDataReader->readTokenVector3D();
		m_uiLevel = pDataReader->readTokenUint32();
		if (getFormatType() == 1)
		{
			setFormatGames(GAME_FLAG_GTA_SA | GAME_FLAG_GTA_IV);
			m_strZoneText = pDataReader->readTokenString();
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_ZONE::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
	case 1:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiZoneType);
		pDataWriter->writeToken(m_vecBottomLeftPosition);
		pDataWriter->writeToken(m_vecTopRightPosition);
		pDataWriter->writeToken(m_uiLevel);
		if (getFormatType() == 1)
		{
			pDataWriter->writeToken(m_strZoneText);
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}